/*
solution:	glib
project:	math
file:		vector3_op.cpp
author:		cj
*/

#include "vector3.h"

namespace M {

	Vector3 Normalize(const Vector3& vector) {
		float length = sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
		return vector / length;
	}

	float Length(const Vector3& vector) {
		return sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
	}

	float Distance(const Vector3& lhp, const Vector3& rhp) {
		return M::Length(rhp - lhp);
	}

	bool LinearlyDependent(const Vector3& u, const Vector3& v) {
		return AlmostEqual(u.x * v.y - u.y * v.x, 0.0f) &&
			   AlmostEqual(u.x * v.z - u.z * v.x, 0.0f);
	}

} // namespace M