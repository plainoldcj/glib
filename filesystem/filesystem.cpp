/*
solution:	glib
project:	filesystem
file:		filesystem.cpp
author:		cj
*/

#include <Windows.h>
#include "../common/common.h"
#include "../common/config.h"
#include "filesystem.h"

namespace FS {

	/* MemFile Impl */

	struct MemFile : public File {
		std::string name;
		char* memory;
		char* ptr;
		int size;
			
		MemFile(const std::string& name, char* const memory, int size) 
			: name(name), memory(memory), ptr(memory), size(size) { }
		~MemFile(void) { }
		
		const std::string& Name(void) const { return name; }
		int Size(void) const { return size; }

		bool Eof(void) const;
		void Read(char* buffer, int size);
		void Gets(char* buffer, int size);
	};

	bool MemFile::Eof(void) const {
		return ptr >= memory + size;
	}

	void MemFile::Read(char* buffer, int size) {
		int i;
		for(i = 0; i < size; ++i) {
			buffer[i] = *ptr;
			ptr++;
		}
	}

	bool IsTerminator(char c) {
		return '\n' == c || '\0' == c || '\r' == c;
	}

	void MemFile::Gets(char* buffer, int size) {
		int i;
		for(i = 0; i < size; ++i) {
			char c = *ptr;
			if(IsTerminator(c)) {
				*ptr = '\0';
				ptr++;
				return;
			}
			buffer[i] = *ptr;
			ptr++;
		}
	}

	/* Disk Impl */

	GEN::Pointer<File> Disk::GetFile(const std::string& filename) {
		FILE* file = fopen(filename.c_str(), "rb");
		if(!file) {
			COM::log << "unable to open file " << filename << std::endl;
			throw COM::FileNotFoundException();
		}

		fseek(file, 0, SEEK_END);
		int size = ftell(file);
		rewind(file);

		char* memory = new char[size];
		fread(memory, size, 1, file);

		fclose(file);

		return GEN::Pointer<File>(new MemFile(filename, memory, size));
	}

	/* Dump Impl */

	const int MAGIC = 0xABCDEF09;
	const int KEY = 0xAB8F9AC2;

	// super simple, yet sufficient
	void Crypto(char* mem, int size) {
		if(4 > size) return;
		int len = size / 4;
		int* ptr = (int*)mem;
		for(int i = 0; i < len; ++i)
			*ptr++ ^= KEY;
	}


	struct DumpHeader {
		int magic;
		int indexOffset;
	};

	void Dump::WriteIndex(void) {
		fseek(_file, _indexOff, SEEK_SET);
		for(indexIt_t indexIt(_index.begin()); _index.end() != indexIt; ++indexIt) {
			fwrite(indexIt->first.c_str(), indexIt->first.length(), 1, _file);
			fwrite(&_delim, sizeof(char), 1, _file);
			fwrite(&indexIt->second, sizeof(int), 1, _file);
		}

		DumpHeader header = { MAGIC, _indexOff };
		rewind(_file);
		fwrite(&header, sizeof(DumpHeader), 1, _file);
	}

	Dump::Dump(void) : _delim(0xFF) {
		const COM::Config& config = COM::Config::Instance();
		if(_debug = config.Get("debug", 0)) {
			COM::log << "Dump::Dump: debug mode enabled, will read files from disk" << std::endl;
		}

		const char* filename = "dump.txt";

		// make sure dump file exists
		FILE* touch = fopen(filename, "a");
		fclose(touch);

		_file = fopen(filename, "rb+");
		if(!_file) {
			COM::log << "unable to open dump" << std::endl;
			throw COM::IOException();
		}

		DumpHeader header;
		if(1 != fread(&header, sizeof(DumpHeader), 1, _file) || MAGIC != header.magic) {
			COM::log << "error reading dump header, creating empty dump" << std::endl;
			header.magic = MAGIC;
			header.indexOffset = sizeof(DumpHeader);
			rewind(_file);
			fwrite(&header, sizeof(DumpHeader), 1, _file);
		} else {
			fseek(_file, header.indexOffset, SEEK_SET);
			while(!feof(_file)) {
				std::string filename;
				int offset;

				while(!feof(_file)) {
					char c;
					fread(&c, sizeof(char), 1, _file);
					if(_delim != c) filename += c;
					else {
						fread(&offset, sizeof(int), 1, _file);
						_index[filename] = offset;
						break;
					}
				}
			}
		}
		_indexOff = header.indexOffset;
	}

	Dump::~Dump(void) {
		WriteIndex();
		fseek(_file, 0, SEEK_END);
	}

	GEN::Pointer<File> Dump::GetFile(const std::string& filename) {
		indexIt_t indexIt(_index.find(filename));
		const bool miss = _index.end() == indexIt;
		if(_debug || miss) {
			FILE* file = fopen(filename.c_str(), "rb");
			if(!file) {
				const unsigned DIR_BUFFER_SIZE = 2048;
				char dir[DIR_BUFFER_SIZE];
				GetCurrentDirectoryA(DIR_BUFFER_SIZE, dir);
				COM::log << "unable to open file " << filename 
					<< " in directory " << dir << std::endl;
				throw COM::FileNotFoundException();
			}

			fseek(file, 0, SEEK_END);
			int size = ftell(file);
			rewind(file);

			char* memory = new char[size];
			fread(memory, size, 1, file);

			fclose(file);

			if(miss) {
				_index[filename] = _indexOff;

				Crypto(memory, size); // encrypt

				fseek(_file, _indexOff, SEEK_SET);
				fwrite(&size, sizeof(int), 1, _file);
				fwrite(memory, size, 1, _file);
				_indexOff = ftell(_file);

				Crypto(memory, size); // decrypt, since memory is passed to file
			}

			return GEN::Pointer<File>(new MemFile(filename, memory, size));
		} else {
			fseek(_file, indexIt->second, SEEK_SET);

			int size;
			fread(&size, sizeof(int), 1, _file);

			char* memory = new char[size];
			fread(memory, size, 1, _file);

			Crypto(memory, size);

			return GEN::Pointer<File>(new MemFile(filename, memory, size));
		}
		return GEN::Pointer<File>();
	}

} // namespace FS