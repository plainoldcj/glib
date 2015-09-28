/*
solution:	glib
project:	math
file:		vector3_mem_inl.h
*/

#pragma once

#include "math.h"
#include "vector3.h"

namespace M {

	M_INLINE Vector3::Vector3(const Vector3& other) : x(other.x), y(other.y), z(other.z) { }

	M_INLINE Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z) { }

	M_INLINE Vector3& Vector3::operator=(const Vector3& other) {
		if(&other == this) return *this;
		x = other.x;
		y = other.y;
		z = other.z;
		return *this;
	}

	M_INLINE Vector3 Vector3::operator-(void) const {
		return Vector3(-x, -y, -z);
	}

	M_INLINE Vector3& Vector3::operator+=(const Vector3& other) {
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	M_INLINE Vector3& Vector3::operator/=(float scalar) {
		x /= scalar;
		y /= scalar;
		z /= scalar;
		return *this;
	}

} // namespace M