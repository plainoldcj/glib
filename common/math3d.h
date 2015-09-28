/*

file: math3d.h
*/

#pragma once

/*
NOTE:
the whole math library is currently a bit messy
and fucked up in terms of layout and namespaces.
ill never fix this, so deal with it.
*/

#include <math.h>
#include <ostream>
#include "common.h"

#define M_PI 3.141f

namespace M {

	static float Deg2Rad(float deg) {
		const float factor = M_PI / 180.0f;
		return deg * factor;
	}

	static float Rad2Deg(float rad) {
		const float factor = 180.0f / M_PI;
		return rad * factor;
	}

} // namespace M

#include "vector.h"
#include "matrix.h"
#include "quaternion.h"
#include "bbox.h"