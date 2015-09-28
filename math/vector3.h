/*
solution:	glib
project:	math
file:		vector3.h
author:		cj
*/

#pragma once

#include <math.h>
#include <iostream>

namespace M {

	struct Vector3 {
		union {
			struct {
				float x, y, z;
			};
			float vec[3];
		};

		Vector3(void) { }
		Vector3(const Vector3& other);
		Vector3(float x, float y, float z);

		Vector3&	operator=(const Vector3& other);
		Vector3		operator-(void) const;
		Vector3&	operator+=(const Vector3& other);
		Vector3&	operator/=(float scalar);

		float Length(void) const;

		void Normalize(void);

		static Vector3 Zero;
	};

	std::ostream& operator<<(std::ostream& stream, const M::Vector3& vector);

	Vector3 operator+(const Vector3& lhp, const Vector3& rhp);
	Vector3 operator-(const Vector3& lhp, const Vector3& rhp);
	Vector3 operator*(float scalar, const Vector3& vector);
	Vector3 operator*(const Vector3& vector, float scalar);
	Vector3 operator/(const Vector3& vector, float scalar);

	Vector3	Negate(const Vector3& vec);
	float	Dot(const Vector3& lhp, const Vector3& rhp);
	Vector3	Cross(const Vector3& lhp, const Vector3& rhp);
	Vector3	Normalize(const Vector3& vector);
	float	Length(const Vector3& vector);
	float	Distance(const Vector3& lhp, const Vector3& rhp);
	bool	LinearlyDependent(const Vector3& u, const Vector3& v);

} // namespace M

#include "vector3_mem_inl.h"
#include "vector3_op_inl.h"