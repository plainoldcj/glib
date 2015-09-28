/*
solution:	glib
project:	spheres
file:		plane.cpp
author:		cj
*/

#include "../system/glx.h"
#include "../math/matrix4.h"
#include "plane.h"

Plane::Plane(float size) : _size(size) { }

void Plane::RenderPreview(void) const {
	M::Matrix4 transformation = M::Mat4::Translate(GetPosition());
	glPushMatrix();
	glMultMatrixf(transformation.mat);

	const float s = _size;
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-s, 0.0f,  s);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-s, 0.0f, -s);
	glTexCoord2f(1.0f, 1.0f); glVertex3f( s, 0.0f, -s);
	glTexCoord2f(0.0f, 1.0f); glVertex3f( s, 0.0f,  s);
	glEnd();

	glPopMatrix();
}