/*
solution:	glib
project:	cubemapping
file:		cubemap.cpp
author:		cj
*/

#include "../filesystem/filesystem.h"
#include "cubemap.h"

CubeMap::CubeMap(const std::string& prefix) {
	R_Init();

	const char* suffixes[] = {
		"_right", "_top", "_back", "_left", "_bottom", "_front"
	};

	glx.ActiveTexture(GL_TEXTURE0);
	for(int i = 0; i < NUM_FACES; ++i) {
		_textures[i] = R_GetTexture(FS::Dump::Instance(), TYPE_TGA, (prefix + suffixes[i] + ".tga").c_str());
	}
}

void CubeMap::Bind(GLuint program) {
	const char* names[] = {
		"texCubePosX", "texCubePosY", "texCubePosZ",
		"texCubeNegX", "texCubeNegY", "texCubeNegZ"
	};
	GLint texLoc = -1;

	for(int i = 0; i < NUM_FACES; ++i) {
		glx.ActiveTexture(GL_TEXTURE0 + R_TEX_CUBE_POS_X + i);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, _textures[i]->id);
		
		texLoc = glx.GetUniformLocation(program, names[i]);
		glx.Uniform1i(texLoc, R_TEX_CUBE_POS_X + i);
	}

	glx.ActiveTexture(GL_TEXTURE0);
}

void CubeMap::Render(void) {
	glDisable(GL_DEPTH_TEST);

	const float size = 1.0f;

	// front
	glBindTexture(GL_TEXTURE_2D, _textures[FRONT_FACE]->id);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, -size, -size);
	glTexCoord2f(1.0f, 0.0f); glVertex3f( size, -size, -size);
	glTexCoord2f(1.0f, 1.0f); glVertex3f( size,  size, -size);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-size,  size, -size);
	glEnd();

	// right
	glBindTexture(GL_TEXTURE_2D, _textures[RIGHT_FACE]->id);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f( size, -size, -size);
	glTexCoord2f(1.0f, 0.0f); glVertex3f( size, -size,  size);
	glTexCoord2f(1.0f, 1.0f); glVertex3f( size,  size,  size);
	glTexCoord2f(0.0f, 1.0f); glVertex3f( size,  size, -size);
	glEnd();

	// back
	glBindTexture(GL_TEXTURE_2D, _textures[BACK_FACE]->id);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-size, -size,  size);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-size,  size,  size);
	glTexCoord2f(0.0f, 1.0f); glVertex3f( size,  size,  size);
	glTexCoord2f(0.0f, 0.0f); glVertex3f( size, -size,  size);
	glEnd();

	// left
	glBindTexture(GL_TEXTURE_2D, _textures[LEFT_FACE]->id);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-size, -size, -size);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-size,  size, -size);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-size,  size,  size);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, -size,  size);
	glEnd();

	// top
	glBindTexture(GL_TEXTURE_2D, _textures[TOP_FACE]->id);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-size,  size, -size);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-size,  size,  size);
	glTexCoord2f(1.0f, 1.0f); glVertex3f( size,  size,  size);
	glTexCoord2f(1.0f, 0.0f); glVertex3f( size,  size, -size);
	glEnd();

	// bottom
	glBindTexture(GL_TEXTURE_2D, _textures[BOTTOM_FACE]->id);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, -size,  size);
	glTexCoord2f(1.0f, 0.0f); glVertex3f( size, -size,  size);
	glTexCoord2f(1.0f, 1.0f); glVertex3f( size, -size, -size);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-size, -size, -size);
	glEnd();

	glEnable(GL_DEPTH_TEST);
}