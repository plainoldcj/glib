/*
solution:	glib
project:	prenderer
file:		framebuffer.cpp
author:		cj
*/

#include "renderbuffer.h"
#include "texture.h"
#include "framebuffer.h"

namespace R {

	Framebuffer::Framebuffer(void) {
		glx.GenFramebuffers(1, &_id);
	}

	Framebuffer::~Framebuffer(void) {
		glx.DeleteFramebuffers(1, &_id);
	}

	void Framebuffer::Attach(GLenum type, const Renderbuffer& renderbuffer) {
		glx.BindFramebuffer(GL_FRAMEBUFFER, _id);
		glx.FramebufferRenderbuffer(GL_FRAMEBUFFER, type, GL_RENDERBUFFER, _id);
		GLenum error = glGetError();
		if(GL_NO_ERROR != error) {
			COM::log << "Framebuffer::Attach (Renderbuffer): glGetError() = " << error << std::endl;
			throw COM::GLException();
		}
		glx.BindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Framebuffer::Attach(GLenum type, const Texture& texture) {
		glx.BindFramebuffer(GL_FRAMEBUFFER, _id);
		glx.FramebufferTexture2D(GL_FRAMEBUFFER, type, GL_TEXTURE_2D,
			texture.GetID(), 0);
		GLenum error = glGetError();
		if(GL_NO_ERROR != error) {
			COM::log << "Framebuffer::Attach (Texture): glGetError() = " << error << std::endl;
			throw COM::GLException();
		}
		glx.BindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Framebuffer::Bind(void) {
		glx.BindFramebuffer(GL_FRAMEBUFFER, _id);
	}

	void UnbindFramebuffer(void) {
		glx.BindFramebuffer(GL_FRAMEBUFFER, 0);
	}

} // namespace R