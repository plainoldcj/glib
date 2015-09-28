/*
solution:	glib
project:	math
file:		intersections.h
author:		cj
*/

#pragma once

#ifndef NULL
#define NULL 0
#endif

namespace M {

	struct Ray;
	struct Sphere;

	namespace IS {

		struct Info {
		};

		bool Analytically(const Ray& ray, const Sphere& sphere, Info* info = NULL);

	} // namespace IS

} // namespace M