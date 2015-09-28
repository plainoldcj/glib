/*
solution:	glib
project:	prenderer
file:		shader.cpp
author:		cj
*/

#include "../common/common.h"
#include "shader.h"

#define LOGBUFFER_SIZE 2048

namespace R {

	Shader::Shader(GEN::Pointer<FS::File> file, GLenum type) {
		int size;
		char* buffer;
		int status;
		char logBuffer[LOGBUFFER_SIZE];

		_id = glx.CreateShader(type);
		if(!_id) {
			COM::log << "creating shader failed, " << glGetError() << std::endl;
			throw COM::InvalidFormatException();
		}

		size = file->Size();

		buffer = (char*)malloc(size);
		if(!buffer) {
			COM::log << "out of memory" << std::endl;
			throw std::bad_alloc();
		}


		file->Read(buffer, size);

		glx.ShaderSource(_id, 1, (const GLchar**)&buffer, &size);
		glx.CompileShader(_id);

		glx.GetShaderiv(_id, GL_COMPILE_STATUS, &status);
		if(GL_TRUE != status) {
			glx.GetShaderInfoLog(_id, LOGBUFFER_SIZE,
				&size, (GLchar*)logBuffer);
			COM::log << "compiling shader " << file->Name() << " failed, log: " << std::endl
				<< logBuffer << std::endl;
			throw COM::GLException();
		}
	}

	Shader::~Shader(void) {
		glx.DeleteShader(_id);
	}

} // namespace R