/*
solution:	glib
project:	raytracer0
file:		entity.h
author:		cj
*/

#pragma once

namespace M {
	struct Ray;
}

namespace RT0 {

	class Entity {
	public:
		virtual ~Entity(void) { }

		virtual bool Intersects(const M::Ray& ray) = 0;
	};

} // namespace RT0