/*
solution:	glib
project:	math
file:		matrix4_mem.cpp
author:		cj
*/

#include "matrix4.h"

namespace M {

	Matrix4::Matrix4(float m00, float m01, float m02, float m03,
					 float m10, float m11, float m12, float m13,
					 float m20, float m21, float m22, float m23,
					 float m30, float m31, float m32, float m33)
					 :	m00(m00), m01(m01), m02(m02), m03(m03),
						m10(m10), m11(m11), m12(m12), m13(m13),
						m20(m20), m21(m21), m22(m22), m23(m23),
						m30(m30), m31(m31), m32(m32), m33(m33) { }

	Matrix4::Matrix4(const Matrix4& other)
		:	m00(other.m00), m01(other.m01), m02(other.m02), m03(other.m03),
			m10(other.m10), m11(other.m11), m12(other.m12), m13(other.m13),
			m20(other.m20), m21(other.m21), m22(other.m22), m23(other.m23),
			m30(other.m30), m31(other.m31), m32(other.m32), m33(other.m33) { }

	Matrix4& Matrix4::operator=(const Matrix4& other) {
		if(&other == this) return *this;
		for(unsigned i = 0; i < 16; ++i)
			mat[i] = other.mat[i];
		return *this;
	}

} // namespace M