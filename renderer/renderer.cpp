/*
solution:	glib
project:	renderer
file:		renderer.cpp
author:		cj
*/

#include "renderer.h"

int R_Init(void) {
	for(int i = 0; i < R_NUM_TEX_LEVELS; ++i) {
		glx.ActiveTexture(GL_TEXTURE0 + i);
		glEnable(GL_TEXTURE_2D);
	}

	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0f);

	return 0;
}

void R_DrawVector(const M::Vector3& vec, float length) {
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(vec.x * length, vec.y * length, vec.z * length);
	glEnd();
}

/* primitives */

void R_DrawCube(float size) {
	glBegin(GL_QUADS);
	
	// front
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, -size, size);
	glTexCoord2f(1.0f, 0.0f); glVertex3f( size, -size, size);
	glTexCoord2f(1.0f, 1.0f); glVertex3f( size,  size, size);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-size,  size, size);

	glEnd();
}