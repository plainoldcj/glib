/*
file: quaternion.h
*/

#pragma once

#include "vector.h"

namespace M {

	struct Vector3;

	struct Quaternion {
		float w, x, y, z;

		Quaternion(void) { }
		Quaternion(float w, float x, float y, float z)
			: w(w), x(x), y(y), z(z) { }
		explicit Quaternion(const Vector3& vector)
			: w(0), x(vector.x), y(vector.y), z(vector.z) { }
	};

	inline Quaternion operator*(const Quaternion& lhp, const Quaternion& rhp) {
		return Quaternion((lhp.w * rhp.w) - (lhp.x * rhp.x) - (lhp.y * rhp.y) - (lhp.z * rhp.z),
						  (lhp.x * rhp.w) + (lhp.w * rhp.x) + (lhp.y * rhp.z) - (lhp.z * rhp.y),
						  (lhp.y * rhp.w) + (lhp.w * rhp.y) + (lhp.z * rhp.x) - (lhp.x * rhp.z),
						  (lhp.z * rhp.w) + (lhp.w * rhp.z) + (lhp.x * rhp.y) - (lhp.y * rhp.x));

	}

	inline Quaternion Conjugate(const Quaternion& quat) {
		return Quaternion(quat.w, -quat.x, -quat.y, -quat.z);
	}

	inline Vector3 RotateVector(const Vector3& vector, const Quaternion& quat) {
		Quaternion r(quat * Quaternion(vector) * Conjugate(quat));
		return Vector3(r.x, r.y, r.z);
	}

} // namespace M