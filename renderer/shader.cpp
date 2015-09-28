/*
solution:	glib
project:	renderer
file:		shader.cpp
author:		cj
*/

#include "../filesystem/filesystem.h"
#include "renderer.h"

#define LOGBUFFER_SIZE 4096

int R_CreateShader(rshader_t* shader, GLenum type, const char* filename) {
	//FILE* file;
	GEN::Pointer<FS::File> file;
	int size;
	char* buffer;
	int status;
	char logBuffer[LOGBUFFER_SIZE];

	COM_assert(NULL != shader);

	shader->valid = false;

	shader->shader = glx.CreateShader(type);
	if(!shader->shader) {
		COM::log << "creating shader failed, " << glGetError() << std::endl;
		return 0;
	}
	
	/*file = fopen(filename, "rb");
	if(!file) {
		COM::log << "unable to open file " << filename << std::endl;
		return 0;
	}*/
	file = FS::Dump::Instance().GetFile(filename);

	/*fseek(file, 0, SEEK_END);
	size = ftell(file);
	fseek(file, 0, SEEK_SET);*/
	size = file->Size();

	buffer = (char*)malloc(size);
	if(!buffer) {
		COM::log << "out of memory" << std::endl;
		return 0;
	}

	//fread(buffer, 1, size, file);
	file->Read(buffer, size);

	glx.ShaderSource(shader->shader, 1, (const GLchar**)&buffer, &size);
	glx.CompileShader(shader->shader);

	glx.GetShaderiv(shader->shader, GL_COMPILE_STATUS, &status);
	if(GL_TRUE != status) {
		glx.GetShaderInfoLog(shader->shader, LOGBUFFER_SIZE,
			&size, (GLchar*)logBuffer);
		COM::log << "compiling shader failed, log: " << std::endl
			<< logBuffer << std::endl;
	}

	shader->valid = true;

	free(buffer);
	//fclose(file);

	return 0;
}

void R_DeleteShader(rshader_t* shader) {
	if(shader->valid) {
		glx.DeleteShader(shader->shader);
		shader->valid = false;
	}
}

int R_LinkProgram(GLuint program) {
	int status;
	char logBuffer[LOGBUFFER_SIZE];
	int size;

	glx.LinkProgram(program);

	glx.GetProgramiv(program, GL_LINK_STATUS, &status);
	if(GL_TRUE != status) {
		glx.GetProgramInfoLog(program, LOGBUFFER_SIZE, &size, (GLchar*)logBuffer);
		COM::log << "linking program failed, log: " << std::endl
			<< logBuffer << std::endl;
		return 1;
	}

	return 0;
}	