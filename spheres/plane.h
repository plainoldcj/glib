/*
solution:	glib
project:	spheres
file:		plane.h
author:		cj
*/

#pragma once

#include "entity.h"

class Plane : public Entity {
private:
	float _size;
public:
	Plane(float size);

	void RenderPreview(void) const;
};