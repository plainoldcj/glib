/*
solution:	glib
project:	system
file:		extensions.h
author:		cj
*/

#pragma once

#include <Windows.h>
#include <gl\GL.h>
#include "glext.h"
#include "../common/uncopyable.h"

namespace SYS { struct Extensions; }

// this is set to the extensions of the currently Used() rendering context
// extern SYS::Extensions glx; (see below)

namespace SYS {

	struct Extensions {
		// bitmap operations
		PFNGLWINDOWPOS2IPROC WindowPos2i;

		// VBOs
		PFNGLGENBUFFERSPROC	GenBuffers;
		PFNGLBINDBUFFERPROC	BindBuffer;
		PFNGLBUFFERDATAPROC	BufferData;

		// Shader
		PFNGLCREATESHADERPROC		CreateShader;
		PFNGLDELETESHADERPROC		DeleteShader;
		PFNGLSHADERSOURCEPROC		ShaderSource;
		PFNGLCOMPILESHADERPROC		CompileShader;
		PFNGLGETSHADERIVPROC		GetShaderiv;
		PFNGLGETSHADERINFOLOGPROC	GetShaderInfoLog;
		PFNGLCREATEPROGRAMPROC		CreateProgram;
		PFNGLATTACHSHADERPROC		AttachShader;
		PFNGLLINKPROGRAMPROC		LinkProgram;
		PFNGLGETPROGRAMIVPROC		GetProgramiv;
		PFNGLGETPROGRAMINFOLOGPROC	GetProgramInfoLog;
		PFNGLUSEPROGRAMPROC			UseProgram;

		PFNGLGETUNIFORMLOCATIONPROC	GetUniformLocation;
		PFNGLUNIFORM1IPROC			Uniform1i;
		PFNGLUNIFORM1FPROC			Uniform1f;
		PFNGLUNIFORM4FPROC			Uniform4f;
		PFNGLUNIFORM3FPROC			Uniform3f;
		PFNGLUNIFORMMATRIX3FVPROC	UniformMatrix3fv;
		PFNGLUNIFORMMATRIX4FVPROC	UniformMatrix4fv;

		PFNGLGETATTRIBLOCATIONPROC			GetAttribLocation;
		PFNGLVERTEXATTRIBPOINTERPROC		VertexAttribPointer;
		PFNGLENABLEVERTEXATTRIBARRAYPROC	EnableVertexAttribArray;
		PFNGLDISABLEVERTEXATTRIBARRAYPROC	DisableVertexAttribArray;

		// Textures
		PFNGLACTIVETEXTUREPROC ActiveTexture;

		// Framebuffer
		PFNGLGENFRAMEBUFFERSPROC			GenFramebuffers;
		PFNGLDELETEFRAMEBUFFERSPROC			DeleteFramebuffers;
		PFNGLBINDFRAMEBUFFERPROC			BindFramebuffer;
		PFNGLFRAMEBUFFERTEXTURE2DPROC		FramebufferTexture2D;
		PFNGLFRAMEBUFFERRENDERBUFFERPROC	FramebufferRenderbuffer;
		PFNGLCHECKFRAMEBUFFERSTATUSPROC		CheckFramebufferStatus;
		PFNGLDRAWBUFFERSPROC				DrawBuffers;

		// Renderbuffer
		PFNGLGENRENDERBUFFERSPROC		GenRenderbuffers;
		PFNGLDELETERENDERBUFFERSPROC	DeleteRenderbuffers;
		PFNGLBINDRENDERBUFFERPROC		BindRenderbuffer;
		PFNGLRENDERBUFFERSTORAGEPROC	RenderbufferStorage;
	};

} // namespace SYS

// putting this here is better for intellisense
extern SYS::Extensions glx;