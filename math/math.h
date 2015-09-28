/*
solution:	glib
project:	math
file:		math.h
author:		cj
*/

#pragma once

#define M_INLINE __forceinline

namespace M {

	const float PI = 3.14159265f;

	template<typename TYPE> TYPE Abs(TYPE value);
	template<typename TYPE> TYPE Min(TYPE lhp, TYPE rhp);
	template<typename TYPE> TYPE Max(TYPE lhp, TYPE rhp);

	float Deg2Rad(float deg);
	float Rad2Deg(float rad);

	bool AlmostEqual(float lhp, float rhp);

} // namespace M

#include "math_inl.h"