/*
solution:	glib
project:	math
file:		ray.h
author:		cj
*/

#pragma once

#include "vector3.h"

namespace M {

	struct Ray {
		M::Vector3 origin;
		M::Vector3 direction;
	};

} // namespace M