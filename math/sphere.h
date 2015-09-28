/*
solution:	glib
project:	math
file:		sphere.h
author:		cj
*/

#pragma once

#include "vector3.h"

namespace M {

	struct Sphere {
		M::Vector3 center;
		float radius;
	};

} // namespace M