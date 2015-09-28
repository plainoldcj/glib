/*
solution:	glib
project:	math
file:		matrix3.h
author:		cj
*/

#pragma once

namespace M {

	struct Vector3;

	struct Matrix3 {
		union {
			struct {
				float m00, m10, m20;
				float m01, m11, m21;
				float m02, m12, m22;
			};
			struct {
				float a, d, g;
				float b, e, h;
				float c, f, i;
			};
			float mat[9];
		};

		Matrix3(void) { }
		Matrix3(float m00, float m01, float m02,
				float m10, float m11, float m12,
				float m20, float m21, float m22);
	};

	Matrix3 operator*(const Matrix3& lhp, const Matrix3& rhp);
	Matrix3 operator/(const Matrix3& mat, float scalar);

	Vector3 Transform(const Matrix3& mat, const Vector3& vec);
	float	Det(const Matrix3& m);
	Matrix3 Inverse(const Matrix3& m);
	Matrix3 RigidInverse(const Matrix3& m);
	void	ToColumns(const M::Matrix3& mat, M::Vector3& A1, M::Vector3& A2, M::Vector3& A3);

	namespace Mat3 {
		Matrix3 FromColumns(const M::Vector3& A1, const M::Vector3& A2, const M::Vector3& A3);

		Matrix3 Identity(void);
		Matrix3 RotateY(float angle);
		Matrix3 RotateAxis(const Vector3& axis, float angle);

	} // namespace Mat3

} // namespace M