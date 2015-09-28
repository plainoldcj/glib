/*
solution:	glib
project:	math
file:		matrix4.h
author:		cj
*/

#pragma once

namespace M {

	struct Vector3;
	struct Matrix3;

	struct Matrix4 {
		union {
			struct {
				float m00, m10, m20, m30;
				float m01, m11, m21, m31;
				float m02, m12, m22, m32;
				float m03, m13, m23, m33;
			};
			float mat[16];
		};

		Matrix4(void) { }
		Matrix4(float m00, float m01, float m02, float m03,
				float m10, float m11, float m12, float m13,
				float m20, float m21, float m22, float m23,
				float m30, float m31, float m32, float m33);
		Matrix4(const Matrix4& other);

		Matrix4& operator=(const Matrix4& other);
	};

	Matrix4 operator*(const Matrix4& lhp, const Matrix4& rhp);

	Vector3 Transform(const Matrix4& mat, const Vector3& vec);

	namespace Mat4 {

		Matrix4 FromRigidTransform(const M::Matrix3& rot, const M::Vector3& tran);

		Matrix4 Identity(void);
		Matrix4 Translate(float x, float y, float z);
		Matrix4 Translate(const M::Vector3& translation);
		Matrix4 RotateX(float angle);
		Matrix4 RotateY(float angle);
		Matrix4 RotateZ(float angle);
		Matrix4 RotateAxis(const Vector3& axis, float angle);

	} // namespace Mat4

} // namespace M