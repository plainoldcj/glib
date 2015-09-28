/*
solution:	glib
project:	math
file:		matrix3_mem.cpp
author:		cj
*/

#include "matrix3.h"

namespace M {

	Matrix3::Matrix3(float m00, float m01, float m02,
					 float m10, float m11, float m12,
					 float m20, float m21, float m22)
					 :	m00(m00), m01(m01), m02(m02),
						m10(m10), m11(m11), m12(m12),
						m20(m20), m21(m21), m22(m22) { }

} // namespace M