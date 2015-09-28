/*
solution:	glib
project:	prenderer
file:		shader.h
author:		cj
*/

#pragma once

#include <string>
#include <map>

#include "../system/glx.h"
#include "../filesystem/filesystem.h"

namespace R {

	class Shader : private GEN::Uncopyable {
	private:
		GLuint _id;
	public:
		enum Type {
			// ... for intellisense
			VERTEX_SHADER = GL_VERTEX_SHADER,
			PIXEL_SHADER = GL_FRAGMENT_SHADER
		};

		Shader(GEN::Pointer<FS::File> file, GLenum type);
		~Shader(void);

		GLuint GetID(void) const { return _id; }
	};

	/*
	i do not provide a ShaderManager class, as you might have
	expected, for two reasons:
	- there is only one way to load a shader from text
	- the expected case is that you want to protect
	your shaders, thus using the dump. in its current implementation
	the dump does alrady check for redundancy, so a manager is
	not necessary.
	*/

} // namespace R