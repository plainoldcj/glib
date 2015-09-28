/*
solution:	glib
project:	system
file:		extensions.cpp
author:		cj
*/

#include "../common/common.h"
#include "opengl.h"

SYS::Extensions glx;

namespace SYS {

	bool IsExtensionSupported(const std::string& which) {
		if(std::string::npos != which.find(' ')) return false;

		const std::string extensions((const char*)glGetString(GL_EXTENSIONS));

		std::string search;
		search += " ";
		search += which;
		search += " ";

		return std::string::npos != extensions.find(which);
	}

	void RenderingContext::LoadExtensions(void) {
		// bitmap operations
		_extensions.WindowPos2i = (PFNGLWINDOWPOS2IPROC)wglGetProcAddress("glWindowPos2i");
		if(NULL == _extensions.WindowPos2i) {
			COM::log << "unable to load glWindowPos2i procedure" << std::endl;
		}

		// VBOs
		_extensions.GenBuffers = (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers");
		if(NULL == _extensions.GenBuffers) {
			COM::log << "unable to load glGenBuffers procedure" << std::endl;
		}
		_extensions.BindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
		if(NULL == _extensions.BindBuffer) {
			COM::log << "unable to load glBindBuffer procedure" << std::endl;
		}
		_extensions.BufferData = (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData");
		if(NULL == _extensions.BufferData) {
			COM::log << "unable to load glBufferData procedure" << std::endl;
		}

		// Shader
		if(!IsExtensionSupported("GL_ARB_shader_objects")) {
			COM::log << "extension GL_ARB_shader_objects is not supported" << std::endl;
		}
		_extensions.CreateShader = (PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader");
		if(NULL == _extensions.CreateShader) {
			COM::log << "unable to load glCreateShader procedure" << std::endl;
		}
		_extensions.DeleteShader = (PFNGLDELETESHADERPROC)wglGetProcAddress("glDeleteShader");
		if(NULL == _extensions.DeleteShader) {
			COM::log << "unable to load glDeleteShader procedure" << std::endl;
		}
		_extensions.ShaderSource = (PFNGLSHADERSOURCEPROC)wglGetProcAddress("glShaderSource");
		if(NULL == _extensions.ShaderSource) {
			COM::log << "unable to load glShaderSource procedure" << std::endl;
		}
		_extensions.CompileShader = (PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader");
		if(NULL == _extensions.CompileShader) {
			COM::log << "unable to load glCompileShader procedure" << std::endl;
		}
		_extensions.GetShaderiv = (PFNGLGETSHADERIVPROC)wglGetProcAddress("glGetShaderiv");
		if(NULL == _extensions.GetShaderiv) {
			COM::log << "unable to load glGetShaderiv procedure" << std::endl;
		}
		_extensions.GetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)wglGetProcAddress("glGetShaderInfoLog");
		if(NULL == _extensions.GetShaderInfoLog) {
			COM::log << "unable to load glGetShaderInfoLog procedure" << std::endl;
		}
		_extensions.CreateProgram = (PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram");
		if(NULL == _extensions.CreateProgram) {
			COM::log << "unable to load glCreateProgram" << std::endl;
		}
		_extensions.AttachShader = (PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader");
		if(NULL == _extensions.AttachShader) {
			COM::log << "unable to load glAttachShader" << std::endl;
		}
		_extensions.LinkProgram = (PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram");
		if(NULL == _extensions.LinkProgram) {
			COM::log << "unable to load glLinkProgram" << std::endl;
		}
		_extensions.GetProgramiv = (PFNGLGETPROGRAMIVPROC)wglGetProcAddress("glGetProgramiv");
		if(NULL == _extensions.GetProgramiv) {
			COM::log << "unable to load glGetProgramiv" << std::endl;
		}
		_extensions.GetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)wglGetProcAddress("glGetProgramInfoLog");
		if(NULL == _extensions.GetProgramInfoLog) {
			COM::log << "unable to load glGetProgramInfoLog" << std::endl;
		}
		_extensions.UseProgram = (PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram");
		if(NULL == _extensions.UseProgram) {
			COM::log << "unable to load glUseProgram" << std::endl;
		}
		
		_extensions.GetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)wglGetProcAddress("glGetUniformLocation");
		if(NULL == _extensions.GetUniformLocation) {
			COM::log << "unable to load glGetUniformLocation" << std::endl;
		}
		_extensions.Uniform1i = (PFNGLUNIFORM1IPROC)wglGetProcAddress("glUniform1i");
		if(NULL == _extensions.Uniform1i) {
			COM::log << "unable to load glUniform1i procedure" << std::endl;
		}
		_extensions.Uniform1f = (PFNGLUNIFORM1FPROC)wglGetProcAddress("glUniform1f");
		if(NULL == _extensions.Uniform1f) {
			COM::log << "unable to load glUniform1f procedure" << std::endl;
		}
		_extensions.Uniform4f = (PFNGLUNIFORM4FPROC)wglGetProcAddress("glUniform4f");
		if(NULL == _extensions.Uniform4f) {
			COM::log << "unable to load glUniform4f procedure" << std::endl;
		}
		_extensions.Uniform3f = (PFNGLUNIFORM3FPROC)wglGetProcAddress("glUniform3f");
		if(NULL == _extensions.Uniform3f) {
			COM::log << "unable to load glUniform3f procedure" << std::endl;
		}
		_extensions.UniformMatrix3fv = (PFNGLUNIFORMMATRIX3FVPROC)wglGetProcAddress("glUniformMatrix3fv");
		if(NULL == _extensions.UniformMatrix3fv) {
			COM::log << "unable to load glUniformMatrix3fv procedure" << std::endl;
		}
		_extensions.UniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)wglGetProcAddress("glUniformMatrix4fv");
		if(NULL == _extensions.UniformMatrix4fv) {
			COM::log << "unable to load glUniformMatrix4fv procedure" << std::endl;
		}
		
		_extensions.GetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)wglGetProcAddress("glGetAttribLocation");
		if(NULL == _extensions.GetAttribLocation) {
			COM::log << "unable to load glGetAttribLocation procedure" << std::endl;
		}
		_extensions.VertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer");
		if(NULL == _extensions.VertexAttribPointer) {
			COM::log << "unable to load glVertexAttribPointer procedure" << std::endl;
		}
		_extensions.EnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glEnableVertexAttribArray");
		if(NULL == _extensions.EnableVertexAttribArray) {
			COM::log << "unable to load glEnableVertexAttribArray procedure" << std::endl;
		}
		_extensions.DisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glDisableVertexAttribArray");
		if(NULL == _extensions.DisableVertexAttribArray) {
			COM::log << "unable to load glDisableVertexAttribArray procedure" << std::endl;
		}

		// Textures
		_extensions.ActiveTexture = (PFNGLACTIVETEXTUREPROC)wglGetProcAddress("glActiveTexture");
		if(NULL == _extensions.ActiveTexture) {
			COM::log << "unable to load glActiveTexture procedure" << std::endl;
		}

		// Framebuffer
		_extensions.GenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC)wglGetProcAddress("glGenFramebuffers");
		if(NULL == _extensions.GenFramebuffers) {
			COM::log << "unable to load glGenFramebuffers procedure" << std::endl;
		}
		_extensions.DeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC)wglGetProcAddress("glDeleteFramebuffers");
		if(NULL == _extensions.DeleteFramebuffers) {
			COM::log << "unable to load glDeleteFramebuffers procedure" << std::endl;
		}
		_extensions.BindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC)wglGetProcAddress("glBindFramebuffer");
		if(NULL == _extensions.BindFramebuffer) {
			COM::log << "unable to load glBindFramebuffer procedure" << std::endl;
		}
		_extensions.FramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC)wglGetProcAddress("glFramebufferTexture2D");
		if(NULL == _extensions.FramebufferTexture2D) {
			COM::log << "unable to load glFramebufferTexture2D procedure" << std::endl;
		}
		_extensions.FramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC)wglGetProcAddress("glFramebufferRenderbuffer");
		if(NULL == _extensions.FramebufferRenderbuffer) {
			COM::log << "unable to load glFramebufferRenderbuffer procedure" << std::endl;
		}
		_extensions.CheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC)wglGetProcAddress("glCheckFramebufferStatus");
		if(NULL == _extensions.CheckFramebufferStatus) {
			COM::log << "unable to load glCheckFramebufferStatus procedure" << std::endl;
		}
		_extensions.DrawBuffers = (PFNGLDRAWBUFFERSPROC)wglGetProcAddress("glDrawBuffers");
		if(NULL == _extensions.DrawBuffers) {
			COM::log << "unable to load glDrawBuffers procedure" << std::endl;
		}

		// Renderbuffer
		_extensions.GenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC)wglGetProcAddress("glGenRenderbuffers");
		if(NULL == _extensions.GenRenderbuffers) {
			COM::log << "unable to load glGenRenderbuffers procedure" << std::endl;
		}
		_extensions.DeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSPROC)wglGetProcAddress("glDeleteRenderbuffers");
		if(NULL == _extensions.DeleteRenderbuffers) {
			COM::log << "unable to load glDeleteRenderbuffers procedure" << std::endl;
		}
		_extensions.BindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC)wglGetProcAddress("glBindRenderbuffer");
		if(NULL == _extensions.BindRenderbuffer) {
			COM::log << "unable to load glBindRenderbuffer procedure" << std::endl;
		}
		_extensions.RenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC)wglGetProcAddress("glRenderbufferStorage");
		if(NULL == _extensions.RenderbufferStorage) {
			COM::log << "unable to load glRenderbufferStorage procedure" << std::endl;
		}
	}

} // namespace SYS