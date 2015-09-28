/*
solution:	glib
project:	math
file:		vector3_mem.cpp
author:		cj
*/

#include "vector3.h"

namespace M {

	Vector3 Vector3::Zero(0.0f, 0.0f, 0.0f);

	float Vector3::Length(void) const {
		return sqrt(x * x + y * y + z * z);
	}

	void Vector3::Normalize(void) {
		const float len = Length();
		x /= len;
		y /= len;
		z /= len;
	}

	std::ostream& operator<<(std::ostream& stream, const M::Vector3& vector) {
		stream << "("
			<< vector.x << ", "
			<< vector.y << ", "
			<< vector.z
			<< ")";
		return stream;
	}

} // namespace M