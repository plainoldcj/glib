/*
solution:	glib
project:	renderer
file:		texture.cpp
author:		cj
*/

#include "../filesystem/filesystem.h"
#include "renderer.h"

rtexture_t* textures[MAX_HASH] = { NULL };

int R_LoadTexTGA(FS::Loader& loader, rtexture_t* tex, const char* filename);

rtexture_t* R_GetTexture(FS::Loader& loader, rtexType_t type, const char* filename) {
	if(R_MAX_PATH <= strlen(filename)) {
		COM::log << "filename " << filename << " is too long" << std::endl;
		return NULL;
	}
	int index = COM_Hash(filename);
	rtexture_t* tex = textures[index];
	if(tex) {
		while(tex && strncmp(filename, tex->filename, R_MAX_PATH)) 
			tex = tex->next;
		if(tex) return tex;
	}

	tex = (rtexture_t*)malloc(sizeof(rtexture_t));
	if(!tex) {
		COM::log << "out of memory loading texture " << filename << std::endl;
		return NULL;
	}

	strncpy(tex->filename, filename, R_MAX_PATH);
	int ret = 1;
	switch(type) {
	case TYPE_TGA:
		ret = R_LoadTexTGA(loader, tex, filename);
		break;
	}

	if(ret) return NULL;

	glGenTextures(1, &tex->id);		
	glBindTexture(GL_TEXTURE_2D, tex->id);
	glTexImage2D(GL_TEXTURE_2D, 0, tex->format, tex->width, tex->height, 0, tex->format,
		GL_UNSIGNED_BYTE, tex->pixelData);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	tex->next = textures[index];
	textures[index] = tex;

	return tex;
}

/* Load TGA */

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

int R_LoadTexTGA(FS::Loader& loader, rtexture_t* tex, const char* filename) {
	//FILE* file;
	GEN::Pointer<FS::File> file;
	TGAHeader_t header;
	int bytesPerPixel;
	int size;

	/*if(0 != fopen_s(&file, filename, "rb")) {
		COM::log << "unable to open file " << filename << std::endl;
		return 1;
	}*/
	file = loader.GetFile(filename);
	
	/*if(1 != fread(&header, sizeof(TGAHeader_t), 1, file)) {
		COM::log << "reading %s failed" << filename << std::endl;
		return 1;
	}*/
	file->Read((char*)&header, sizeof(TGAHeader_t));

	if(0 != memcmp(&header.fileSpec, &supportedFileSpec, sizeof(FileSpec_t))) {
		COM::log << filename << " invalid tga format. currently only uncompressed 24bit rga, 32bit rgba files are supported"
					<< std::endl;
		return 1;
	}

	if(0 != header.imageSpec.originx || 0 != header.imageSpec.originy) {
		COM::log << filename << " tga image origin must be 0,0" << std::endl;
		return 1;
	}

	// TODO: test for non-power-of-2 textures
	if(0 == header.imageSpec.width || 0 == header.imageSpec.height) {
		COM::log << filename << " has invalid size ("
					<< header.imageSpec.width << ", "
					<< header.imageSpec.height << ")"
					<< std::endl;
		return 1;
	}

	switch(header.imageSpec.bitsPerPixel) {
	case 24:
		tex->format = GL_RGB;
		break;
	case 32:
		tex->format = GL_RGBA;
		break;
	default:
		COM::log << filename << " must have either 24 or 32bit color depth" << std::endl;
		return 1;
	}

	bytesPerPixel = header.imageSpec.bitsPerPixel / 8;
	size = bytesPerPixel *
		header.imageSpec.width *
		header.imageSpec.height;

	tex->pixelData = (COM::byte_t*)malloc(size);
	if(NULL == tex->pixelData) {
		COM::log << "out of memory loading " << filename << std::endl;
		return 1;
	}

	/*if(1 != fread(tex->pixelData, size, 1, file)) {
		COM::log << "reading image data of " << filename << " failed" << std::endl;
		return 1;
	}*/
	file->Read((char*)tex->pixelData, size);

	for(int i = 0; i < size; i += bytesPerPixel) {
		std::swap(tex->pixelData[i], tex->pixelData[i + 2]);
	}

	tex->width = header.imageSpec.width;
	tex->height = header.imageSpec.height;

	//fclose(file);

	return 0;
}
