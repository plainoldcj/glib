/*
solution:	glib
project:	filesystem
file:		filesystem.h
author:		cj
*/

#pragma once

#include <string>
#include <map>
#include <cstdio>
#include "../common/pointer.h"
#include "../common/singleton.h"

namespace FS {

	class File {
	public:
		virtual ~File(void) { }

		virtual const std::string& Name(void) const = 0;
		virtual int Size(void) const = 0;

		virtual bool Eof(void) const = 0;
		virtual void Read(char* buffer, int size) = 0;
		virtual void Gets(char* buffer, int size) = 0;
	};

	class Loader {
	public:
		virtual ~Loader(void) { }

		virtual GEN::Pointer<File> GetFile(const std::string& filename) = 0;
	};

	class Disk : public Loader, public GEN::Singleton<Disk> {
		friend class GEN::Singleton<Disk>;
	public:
		GEN::Pointer<File> GetFile(const std::string& filename);
	};

	class Dump : public Loader, public GEN::Singleton<Dump> {
		friend class GEN::Singleton<Dump>;
	private:
		typedef std::map<std::string, int>::iterator indexIt_t;

		bool _debug;
		const char _delim;
		FILE* _file;
		int _indexOff;
		std::map<std::string, int> _index;

		void WriteIndex(void);
	public:
		Dump(void);
		~Dump(void);

		GEN::Pointer<File> GetFile(const std::string& filename);
	};

} // namespace FS