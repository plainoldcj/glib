/*
solution:	glib
project:	raytracer0
file:		world.h
author:		cj
*/

#pragma once

namespace M {
	struct Ray;
}

namespace RT0 {

	class World {
	public:
		virtual ~World(void) { }

		virtual void Trace(const M::Ray& ray) = 0;
	};

} // namespace RT0