/*
solution:	glib
project:	prenderer
file:		program.cpp
author:		cj
*/

#include "../common/common.h"
#include "../common/math3d.h"
#include "../prenderer/light.h"
#include "program.h"

#define LOGBUFFER_SIZE 2048

namespace R {

	void Program::Link(void) const {
		int status;
		char logBuffer[LOGBUFFER_SIZE];
		int size;

		glx.LinkProgram(_id);

		glx.GetProgramiv(_id, GL_LINK_STATUS, &status);
		if(GL_TRUE != status) {
			glx.GetProgramInfoLog(_id, LOGBUFFER_SIZE, &size, (GLchar*)logBuffer);
			COM::log << "linking program failed, log: " << std::endl
				<< logBuffer << std::endl;
			throw COM::GLException();
		}

		_linked = true;
	}

	Program::Program(void) : _linked(false) { 
		_id = glx.CreateProgram();
		if(!_id) {
			COM::log << "unable to create program" << std::endl;
			throw COM::GLException();
		}
	}

	void Program::Attach(const Shader& shader) {
		_linked = false;
		glx.AttachShader(_id, shader.GetID());
	}

	void Program::Attach(const std::string& filename, GLenum type) {
		Shader shader(FS::Dump::Instance().GetFile(filename), type);
		Attach(shader);
	}

	void Program::Use(void) const {
		if(!_linked) Link();
		glx.UseProgram(_id);
	}

	GLint Program::GetUniformLocation(const char* name) const {
		if(!_linked) Link();
		GLint loc = glx.GetUniformLocation(_id, name);
		if(0 > loc) {
			COM::log << "Program::GetUniformLocation: uniform " << name
				<< " does not exist." << std::endl;
			// do not throw here, since this is not really an issue. the
			// driver optimizes uniforms out all the time...
			// throw COM::GLException();
		}
		return loc;
	}

	GLint Program::GetAttributeLocation(const char* name) const {
		if(!_linked) Link();
		GLint loc = glx.GetAttribLocation(_id, name);
		if(0 > loc) {
			COM::log << "Program::GetAttributeLocation: attribute " << name
				<< " does not exist." << std::endl;
			throw COM::GLException();
		}
		return loc;
	}

	void Program::SetUniform(const char* name, int value) {
		GLint loc = GetUniformLocation(name);
		if(0 <= loc) glx.Uniform1i(loc, value);
	}

	void Program::SetUniform(const char* name, float value) {
		GLint loc = GetUniformLocation(name);
		if(0 <= loc) glx.Uniform1f(loc, value);
	}

	void Program::SetUniform(const char* name, const M::Vector3& vector) {
		GLint loc = GetUniformLocation(name);
		if(0 <= loc) glx.Uniform3f(loc, vector.x, vector.y, vector.z);
	}

	void Program::SetUniform(const char* name, const M::Matrix3& matrix) {
		GLint loc = GetUniformLocation(name);
		if(0 <= loc) glx.UniformMatrix3fv(loc, 1, GL_FALSE, matrix.mat);
	}

	void Program::SetUniform(const char* name, const M::Matrix4& matrix) {
		GLint loc = GetUniformLocation(name);
		if(0 <= loc) glx.UniformMatrix4fv(loc, 1, GL_FALSE, matrix.mat);
	}

	void Program::SetUniform(const char* name, const R::Color& color) {
		GLint loc = GetUniformLocation(name);
		if(0 <= loc) glx.Uniform4f(loc, color.r, color.g, color.b, color.a);
	}

	void UnbindProgram(void) {
		glx.UseProgram(0);
	}

} // namespace R