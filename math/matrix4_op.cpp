/*
solution:	glib
project:	math
file:		matrix4_op.cpp
author:		cj
*/

#include "vector3.h"
#include "matrix3.h"
#include "matrix4.h"

namespace M {

	Matrix4 operator*(const Matrix4& lhp, const Matrix4& rhp) {
		Matrix4 mat;
		for(int i = 0; i < 4; ++i) {
			for(int j = 0; j < 4; ++j) {
				float el = 0.0f;
				for(int k = 0; k < 4; ++k) {
					el += lhp.mat[i + 4 * k] * rhp.mat[k + 4 * j];
				}
				mat.mat[i + 4 * j] = el;
			}
		}
		return mat;
	}

	Vector3 Transform(const Matrix4& mat, const Vector3& vec) {
		Vector3 ret;
		for(int i = 0; i < 3; ++i) {
			float el = 0.0f;
			for(int j = 0; j < 3; ++j) {
				el += mat.mat[i + 4 * j] * vec.vec[j];
			}
			ret.vec[i] = el + mat.mat[i + 12];
		}
		return ret;
	}

	namespace Mat4 {

		Matrix4 FromRigidTransform(const M::Matrix3& rot, const M::Vector3& tran) {
			return Matrix4(rot.m00, rot.m01, rot.m02, tran.x,
							rot.m10, rot.m11, rot.m12, tran.y,
							rot.m20, rot.m21, rot.m22, tran.z,
							0.0f, 0.0f, 0.0f, 1.0f);
		}

		Matrix4 Identity(void) {
			return Matrix4(1.0f, 0.0f, 0.0f, 0.0f,
							0.0f, 1.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 1.0f, 0.0f,
							0.0f, 0.0f, 0.0f, 1.0f);
		}

		Matrix4 Translate(float x, float y, float z) {
			return Matrix4(1.0f, 0.0f, 0.0f, x,
						   0.0f, 1.0f, 0.0f, y,
						   0.0f, 0.0f, 1.0f, z,
						   0.0f, 0.0f, 0.0f, 1.0f);
		}

		Matrix4 Translate(const M::Vector3& translation) {
			return Translate(translation.x, translation.y, translation.z);
		}

		Matrix4 RotateX(float angle) {
			const float rad = Deg2Rad(angle);
			const float sina = sinf(rad);
			const float cosa = cosf(rad);

			return Matrix4(1.0f, 0.0f, 0.0f, 0.0f,
						   0.0f, cosa, sina, 0.0f,
						   0.0f, -sina, cosa, 0.0f,
						   0.0f, 0.0f, 0.0f, 1.0f);
		}

		Matrix4 RotateY(float angle) {
			const float rad = Deg2Rad(angle);
			const float sina = sinf(rad);
			const float cosa = cosf(rad);

			return Matrix4(cosa, 0.0f, -sina, 0.0f,
						   0.0f, 1.0f, 0.0f, 0.0f,
						   sina, 0.0f, cosa, 0.0f,
						   0.0f, 0.0f, 0.0f, 1.0f);
		}

		Matrix4 RotateZ(float angle) {
			const float rad = Deg2Rad(angle);
			const float sina = sinf(rad);
			const float cosa = cosf(rad);

			return Matrix4(cosa, sina, 0.0f, 0.0f,
						   -sina, cosa, 0.0f, 0.0f,
						   0.0f, 0.0f, 1.0f, 0.0f,
						   0.0f, 0.0f, 0.0f, 1.0f);
		}

		Matrix4 RotateAxis(const Vector3& axis, float angle) {
			const float rad = Deg2Rad(angle);
			const float sina = sinf(rad);
			const float cosa = cosf(rad);
			const float oneMinusCos = 1.0f - cosa;

			const float xyT = axis.x * axis.y * oneMinusCos;
			const float xzT = axis.x * axis.z * oneMinusCos;
			const float yzT = axis.y * axis.z * oneMinusCos;

			const float xT = axis.x * sina;
			const float yT = axis.y * sina;
			const float zT = axis.z * sina;

			return Matrix4(axis.x * axis.x * oneMinusCos + cosa, xyT - zT, xzT + yT, 0.0f,
						   xyT + zT, axis.y * axis.y * oneMinusCos + cosa, yzT - xT, 0.0f,
						   xzT - yT, yzT + xT, axis.z * axis.z * oneMinusCos + cosa, 0.0f,
						   0.0f, 0.0f, 0.0f, 1.0f);
		}

	} // namespace Mat4

} // namespace M