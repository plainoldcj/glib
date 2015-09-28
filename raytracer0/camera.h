/*
solution:	glib
project:	raytracer0
file:		camera.h
author:		cj
*/

#pragma once

namespace RT0 {

	class Camera {
	public:
		virtual void SpawnPrimaryRay(float x, float y) = 0;
	};

} // namespace RT0