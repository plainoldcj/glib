/*
solution:	glib
project:	spheres
file:		entity.h
author:		cj
*/

#pragma once

#include "../math/vector3.h"

class Entity {
private:
	M::Vector3 _position;
public:
	Entity(void) : _position(0.0f, 0.0f, 0.0f) { }

	void SetPosition(const M::Vector3& position) { _position = position; }
	const M::Vector3& GetPosition(void) const { return _position; }

	virtual void RenderPreview(void) const = 0;
};