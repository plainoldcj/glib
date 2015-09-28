/*
solution:	glib
project:	prenderer
file:		program.h
author:		cj
*/

#pragma once

#include "shader.h"

namespace M {
	struct Vector3;
	struct Matrix3;
	struct Matrix4;
}

namespace R {
	struct Color;
}

namespace R {

	class Program : private GEN::Uncopyable {
	private:
		GLuint _id;
		mutable bool _linked;

		void Link(void) const;
	public:
		Program(void);
		~Program(void) { }

		GLuint GetID(void) const { return _id; }

		void Attach(const Shader& shader);
		void Attach(const std::string& filename, GLenum type);

		void Use(void) const;

		GLint GetUniformLocation(const char* name) const;
		GLint GetAttributeLocation(const char* name) const;

		void SetUniform(const char* name, int value);
		void SetUniform(const char* name, float value);
		void SetUniform(const char* name, const M::Vector3& vector);
		void SetUniform(const char* name, const M::Matrix3& matrix);
		void SetUniform(const char* name, const M::Matrix4& matrix);
		void SetUniform(const char* name, const R::Color& color);
	};

	void UnbindProgram(void);

} // namespace R