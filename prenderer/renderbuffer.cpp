/*
solution:	glib
project:	prenderer
file:		renderbuffer.cpp
author:		cj
*/

#include "renderbuffer.h"

namespace R {

	Renderbuffer::Renderbuffer(GLenum type, int width, int height)
		: _width(width), _height(height)
	{
		glx.GenRenderbuffers(1, &_id);
		glx.BindRenderbuffer(GL_RENDERBUFFER, _id);
		glx.RenderbufferStorage(GL_RENDERBUFFER, type, width, height);
	}

	Renderbuffer::~Renderbuffer(void) {
		glx.DeleteRenderbuffers(1, &_id);
	}

} // namespace R