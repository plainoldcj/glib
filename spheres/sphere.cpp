/*
solution:	glib
project:	spheres
file:		sphere.cpp
author:		cj
*/

#include "../common/config.h"
#include "../math/matrix4.h"
#include "sphere.h"

Sphere::Sphere(float radius) : _radius(radius) {
	COM::Config& config = COM::Config::Instance();
	_numSlices = config.Get("slices", 10);
	_numStacks = config.Get("stacks", 10);

	_quadric = gluNewQuadric();
	if(NULL == _quadric) {
		throw std::bad_alloc();
	}
}

Sphere::~Sphere(void) {
	gluDeleteQuadric(_quadric);
}

void Sphere::RenderPreview(void) const {
	M::Matrix4 transformation = M::Mat4::Translate(GetPosition());
	glPushMatrix();
	glMultMatrixf(transformation.mat);

	gluSphere(_quadric, _radius, _numSlices, _numStacks);

	glPopMatrix();
}