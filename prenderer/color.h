/*
solution:	glib
project:	prenderer
file:		color.h
author:		cj
*/

#pragma once

namespace R {

	struct Color {
		float r, g, b, a;

		Color(void) : r(0.0f), g(0.0f), b(0.0f), a(1.0f) { }
		Color(float r, float g, float b, float a = 1.0f) : r(r), g(g), b(b), a(a) { }
		Color(const Color& other) : r(other.r), g(other.g), b(other.b), a(other.a) { }
		~Color(void) { }

		Color& operator=(const Color& other) {
			if(&other == this) return *this;
			r = other.r;
			g = other.g;
			b = other.b;
			a = other.a;
			return *this;
		}
	};

} // namespace R