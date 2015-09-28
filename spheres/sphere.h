/*
solution:	glib
project:	spheres
file:		sphere.h
author:		cj
*/

#pragma once

#include "../system/glx.h"
#include "entity.h"

class Sphere : public Entity {
private:
	float _radius;
	int _numSlices;
	int _numStacks;

	GLUquadric* _quadric;
public:
	Sphere(float radius);
	~Sphere(void);

	void RenderPreview(void) const;
};