/*
solution:	glib
project:	prenderer
file:		tga_texalloc.cpp
author:		cj
*/

#include "texture.h"

#define TYPE_UNCOMPRESSED_RGB24 2

typedef unsigned short tgaWord_t;

#pragma pack(push, 1)

typedef struct {
	COM::byte_t imageId;
	COM::byte_t colorMapType;
	COM::byte_t imageType;

	tgaWord_t	colorMapStart;
	tgaWord_t	colorMapLength;
	COM::byte_t colorMapEntrySize;
} FileSpec_t;

typedef struct {
	tgaWord_t originx;
	tgaWord_t originy;

	tgaWord_t	width;
	tgaWord_t	height;
	COM::byte_t	bitsPerPixel;

	COM::byte_t attributes;
} ImageSpec_t;

typedef struct {
	FileSpec_t fileSpec;
	ImageSpec_t imageSpec;
} TGAHeader_t;

#pragma pack(pop)

const FileSpec_t supportedFileSpec = {
	0, 0, TYPE_UNCOMPRESSED_RGB24, 0, 0, 0
};


R::Texture* AllocTGA(GEN::Pointer<FS::File> file) {
	TGAHeader_t header;
	int bytesPerPixel;
	int size;

	file->Read((char*)&header, sizeof(TGAHeader_t));

	if(0 != memcmp(&header.fileSpec, &supportedFileSpec, sizeof(FileSpec_t))) {
		COM::log << file->Name() << " invalid tga format. currently only uncompressed 24bit rga, 32bit rgba files are supported"
					<< std::endl;
		throw COM::InvalidFormatException();
	}

	if(0 != header.imageSpec.originx || 0 != header.imageSpec.originy) {
		COM::log << file->Name()  << " tga image origin must be 0,0" << std::endl;
		throw COM::InvalidFormatException();
	}

	// TODO: test for non-power-of-2 textures
	if(0 == header.imageSpec.width || 0 == header.imageSpec.height) {
		COM::log << file->Name()  << " has invalid size ("
					<< header.imageSpec.width << ", "
					<< header.imageSpec.height << ")"
					<< std::endl;
		throw COM::InvalidFormatException();
	}

	GLenum format;
	switch(header.imageSpec.bitsPerPixel) {
	case 24:
		format = GL_RGB;
		break;
	case 32:
		format = GL_RGBA;
		break;
	default:
		COM::log << file->Name()  << " must have either 24 or 32bit color depth" << std::endl;
		throw COM::InvalidFormatException();
	}

	bytesPerPixel = header.imageSpec.bitsPerPixel / 8;
	size = bytesPerPixel *
		header.imageSpec.width *
		header.imageSpec.height;

	COM::byte_t* pixelData = (COM::byte_t*)malloc(size);
	if(NULL == pixelData) {
		COM::log << "out of memory loading " << file->Name()  << std::endl;
		throw std::bad_alloc();
	}

	file->Read((char*)pixelData, size);

	for(int i = 0; i < size; i += bytesPerPixel) {
		std::swap(pixelData[i], pixelData[i + 2]);
	}

	int width = header.imageSpec.width;
	int height = header.imageSpec.height;

	R::Texture* texture = new R::Texture(width, height, format, pixelData);

	free(pixelData);
	return texture;
}

R::RegisterTexAlloc registerTGA("tga", AllocTGA);