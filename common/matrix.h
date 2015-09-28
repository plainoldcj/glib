/*
file: matrix.h
*/

#pragma once

namespace M {

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
				float m20, float m21, float m22)
				: m00(m00), m01(m01), m02(m02),
				  m10(m10), m11(m11), m12(m12),
				  m20(m20), m21(m21), m22(m22) { }
	};

	static Matrix3 operator*(const Matrix3& lhp, const Matrix3& rhp) {
		Matrix3 mat;
		for(int i = 0; i < 3; ++i) {
			for(int j = 0; j < 3; ++j) {
				float el = 0.0f;
				for(int k = 0; k < 3; ++k) {
					el += lhp.mat[i + 3 * k] * rhp.mat[k + 3 * j];
				}
				mat.mat[i + 3 * j] = el;
			}
		}
		return mat;
	}

	static Matrix3 operator/(const Matrix3& mat, float scalar) {
		return Matrix3(mat.m00 / scalar, mat.m01 / scalar, mat.m02 / scalar,
					   mat.m10 / scalar, mat.m11 / scalar, mat.m12 / scalar,
					   mat.m20 / scalar, mat.m21 / scalar, mat.m22 / scalar);
	}

	namespace Mat3 {

		static Vector3 Transform(const Matrix3& mat, const Vector3& vec) {
			Vector3 ret;
			for(int i = 0; i < 3; ++i) {
				float el = 0.0f;
				for(int j = 0; j < 3; ++j) {
					el += mat.mat[i + 3 * j] * vec.vec[j];
				}
				ret.vec[i] = el;
			}
			return ret;
		}

		static float Det(const Matrix3& m) {
			return m.a * (m.e * m.i - m.f * m.h)
				- m.b * (m.d * m.i - m.f * m.g)
				+ m.c * (m.d * m.h - m.e * m.g);
		}

		static Matrix3 Inverse(const Matrix3& m) {
			Matrix3 inv(m.e * m.i - m.f * m.h, m.c * m.h - m.b * m.i, m.b * m.f - m.c * m.e,
						m.f * m.g - m.d * m.i, m.a * m.i - m.c * m.g, m.c * m.d - m.a * m.f,
						m.d * m.h - m.e * m.g, m.b * m.g - m.a * m.h, m.a * m.e - m.b * m.d);
			return inv / Det(m);
		}

		static Matrix3 RigidInverse(const Matrix3& m) {
			return Matrix3(m.m00, m.m10, m.m20,
						   m.m01, m.m11, m.m21,
						   m.m02, m.m12, m.m22);
		}

		static void ToColumns(const M::Matrix3& mat, M::Vector3& A1, M::Vector3& A2, M::Vector3& A3) {
			A1.x = mat.m00;
			A1.y = mat.m10;
			A1.z = mat.m20;

			A2.x = mat.m01;
			A2.y = mat.m11;
			A2.z = mat.m21;

			A3.x = mat.m02;
			A3.y = mat.m12;
			A3.z = mat.m22;
		}

		static Matrix3 FromColumns(const M::Vector3& A1, const M::Vector3& A2, const M::Vector3& A3) {
			return Matrix3(A1.x, A2.x, A3.x,
						   A1.y, A2.y, A3.y,
						   A1.z, A2.z, A3.z);
		}

		static Matrix3 Identity(void) {
			return Matrix3(1.0f, 0.0f, 0.0f,
						   0.0f, 1.0f, 0.0f,
						   0.0f, 0.0f, 1.0f);
		}

		static Matrix3 RotateY(float angle) {
			const float rad = Deg2Rad(angle);
			const float sina = sinf(rad);
			const float cosa = cosf(rad);

			return Matrix3(cosa, 0.0f, -sina,
						   0.0f, 1.0f, 0.0f,
						   sina, 0.0f, cosa);
		}

		static Matrix3 RotateAxis(const Vector3& axis, float angle) {
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

			return Matrix3(axis.x * axis.x * oneMinusCos + cosa, xyT - zT, xzT + yT,
						   xyT + zT, axis.y * axis.y * oneMinusCos + cosa, yzT - xT,
						   xzT - yT, yzT + xT, axis.z * axis.z * oneMinusCos + cosa);
		}

	} // namespace Mat3

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
				float m30, float m31, float m32, float m33)
				: m00(m00), m01(m01), m02(m02), m03(m03),
				  m10(m10), m11(m11), m12(m12), m13(m13),
				  m20(m20), m21(m21), m22(m22), m23(m23),
				  m30(m30), m31(m31), m32(m32), m33(m33) { }

		Matrix4& operator=(const Matrix4& other) {
			if(&other == this) return *this;
			for(unsigned i = 0; i < 16; ++i)
				mat[i] = other.mat[i];
			return *this;
		}
	};

	static Matrix4 operator*(const Matrix4& lhp, const Matrix4& rhp) {
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

	namespace Mat4 {

		static M::Matrix4 FromRigidTransform(const M::Matrix3& rot, const M::Vector3& tran) {
			return Matrix4(rot.m00, rot.m01, rot.m02, tran.x,
							rot.m10, rot.m11, rot.m12, tran.y,
							rot.m20, rot.m21, rot.m22, tran.z,
							0.0f, 0.0f, 0.0f, 1.0f);
		}

		static Matrix4 Identity(void) {
			return Matrix4(1.0f, 0.0f, 0.0f, 0.0f,
							0.0f, 1.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 1.0f, 0.0f,
							0.0f, 0.0f, 0.0f, 1.0f);
		}

		static Vector3 Transform(const Matrix4& mat, const Vector3& vec) {
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

		static Matrix4 Translate(float x, float y, float z) {
			return Matrix4(1.0f, 0.0f, 0.0f, x,
						   0.0f, 1.0f, 0.0f, y,
						   0.0f, 0.0f, 1.0f, z,
						   0.0f, 0.0f, 0.0f, 1.0f);
		}

		static Matrix4 Translate(const M::Vector3& translation) {
			return Translate(translation.x, translation.y, translation.z);
		}

		static Matrix4 RotateX(float angle) {
			const float rad = Deg2Rad(angle);
			const float sina = sinf(rad);
			const float cosa = cosf(rad);

			return Matrix4(1.0f, 0.0f, 0.0f, 0.0f,
						   0.0f, cosa, sina, 0.0f,
						   0.0f, -sina, cosa, 0.0f,
						   0.0f, 0.0f, 0.0f, 1.0f);
		}

		static Matrix4 RotateY(float angle) {
			const float rad = Deg2Rad(angle);
			const float sina = sinf(rad);
			const float cosa = cosf(rad);

			return Matrix4(cosa, 0.0f, -sina, 0.0f,
						   0.0f, 1.0f, 0.0f, 0.0f,
						   sina, 0.0f, cosa, 0.0f,
						   0.0f, 0.0f, 0.0f, 1.0f);
		}

		static Matrix4 RotateZ(float angle) {
			const float rad = Deg2Rad(angle);
			const float sina = sinf(rad);
			const float cosa = cosf(rad);

			return Matrix4(cosa, sina, 0.0f, 0.0f,
						   -sina, cosa, 0.0f, 0.0f,
						   0.0f, 0.0f, 1.0f, 0.0f,
						   0.0f, 0.0f, 0.0f, 1.0f);
		}

		static Matrix4 RotateAxis4(const Vector3& axis, float angle) {
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

		static Matrix4 RigidInverse(const Matrix4& mat) {
			Matrix4 irot = Matrix4(mat.m00, mat.m10, mat.m20, 0.0f,
								   mat.m01, mat.m11, mat.m21, 0.0f,
								   mat.m02, mat.m12, mat.m22, 0.0f,
								   0.0f, 0.0f, 0.0f, 1.0f);
			Matrix4 itrans = Translate(-mat.m03, -mat.m13, -mat.m23);
			return irot * itrans;
		}

	} // namespace Mat4

} // namespace M