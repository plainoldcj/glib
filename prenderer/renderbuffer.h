/*
solution:	glib
project:	prenderer
file:		renderbuffer.h
author:		cj
*/

#pragma once

#include "../common/uncopyable.h"
#include "../system/glx.h"

namespace R {

	class Renderbuffer : private GEN::Uncopyable {
	private:
		GLuint _id;
		int _width;
		int _height;
	public:
		enum Type {
			// ... for intellisense
			DEPTH_BUFFER = GL_DEPTH_COMPONENT
		};

		Renderbuffer(GLenum type, int width, int height);
		~Renderbuffer(void);

		GLuint GetID(void) const { return _id; }
		int Width(void) const { return _width; }
		int Height(void) const { return _height; }
	};

} // namespace R