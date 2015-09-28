/*
solution:	glib
project:	prenderer
file:		framebuffer.h
author:		cj
*/

#pragma once

#include "../system/glx.h"

namespace R {

	class Renderbuffer;
	class Texture;

	class Framebuffer {
	private:
		GLuint _id;
	public:
		enum Type {
			DEPTH_BUFFER = GL_DEPTH_ATTACHMENT,
			COLOR_BUFFER_0 = GL_COLOR_ATTACHMENT0,
			COLOR_BUFFER_1 = GL_COLOR_ATTACHMENT1,
			COLOR_BUFFER_2 = GL_COLOR_ATTACHMENT2,
			COLOR_BUFFER_3 = GL_COLOR_ATTACHMENT3
		};

		Framebuffer(void);
		~Framebuffer(void);

		void Attach(GLenum type, const Renderbuffer& renderbuffer);
		void Attach(GLenum type, const Texture& texture);
		void Bind(void);
	};

	void UnbindFramebuffer(void);

} // namespace R