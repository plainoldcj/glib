/*
solution:	glib
project:	math
file:		math_inl.h
author:		cj
*/

#pragma once

#include "math.h"

namespace M {

	template<typename TYPE>
	M_INLINE TYPE Abs(TYPE value) {
		return value < 0 ? -value : value;
	}

	template<>
	M_INLINE float Abs<float>(float value) {
		return value < 0.0f ? -value : value;
	}

	template<typename TYPE>
	M_INLINE TYPE Min(TYPE lhp, TYPE rhp) {
		return lhp < rhp ? lhp : rhp;
	}

	template<typename TYPE>
	M_INLINE TYPE Max(TYPE lhp, TYPE rhp) {
		return lhp > rhp ? lhp : rhp;
	}

	M_INLINE float Deg2Rad(float deg) {
		const float factor = PI / 180.0f;
		return deg * factor;
	}

	M_INLINE float Rad2Deg(float rad) {
		const float factor = 180.0f / PI;
		return rad * factor;
	}

	M_INLINE bool AlmostEqual(float lhp, float rhp) {
		const float epsilon = 0.00001f;
		return Abs(lhp - rhp) < epsilon;
	}

} // namespace M