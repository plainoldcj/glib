/*
solution:	glib
project:	system
file:		opengl.cpp
author:		cj
*/

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

#include "window.h"
#include "opengl.h"

namespace SYS {

	DeviceContext::DeviceContext(Window& window)
		: _windowHandle(window.GetNativeHandle()) 
	{
		_contextHandle = GetDC(_windowHandle);
	}

	DeviceContext::~DeviceContext(void) {
		ReleaseDC(_windowHandle, _contextHandle);
	}

	GLDeviceContext::GLDeviceContext(Window& window) : DeviceContext(window) {
		static PIXELFORMATDESCRIPTOR formatDesc = {
			sizeof(PIXELFORMATDESCRIPTOR),
			1,
			PFD_DRAW_TO_WINDOW |
			PFD_SUPPORT_OPENGL |
			PFD_DOUBLEBUFFER |
			PFD_TYPE_RGBA,
			24,
			0, 0, 0, 0, 0, 0,
			0,
			0,
			0,
			0, 0, 0, 0,
			32,
			0,
			0,
			PFD_MAIN_PLANE,
			0,
			0, 0, 0
		};

		int format = ChoosePixelFormat(_contextHandle, &formatDesc);
		if(!format) throw PixelFormatException();

		if(!SetPixelFormat(_contextHandle, format, &formatDesc)) {
			throw PixelFormatException();
		}
	}

	RenderingContext::RenderingContext(Window& window) 
		: _contextHandle(NULL), _deviceContext(window), _initialized(false) 
	{
		_contextHandle = wglCreateContext(_deviceContext.GetNativeHandle());
	}

	RenderingContext::~RenderingContext(void) {
		if(NULL != _contextHandle) {
			wglMakeCurrent(NULL, NULL);
			wglDeleteContext(_contextHandle);
		}
	}

	void RenderingContext::Use(void) {
		wglMakeCurrent(_deviceContext.GetNativeHandle(), _contextHandle);

		if(!_initialized) {
			PrepareFont();
			LoadExtensions();
		}

		glx = _extensions;
	}

	void RenderingContext::Flip(void) {
		wglSwapLayerBuffers(_deviceContext.GetNativeHandle(), WGL_SWAP_MAIN_PLANE);
	}

	bool IsRenderingContextActive(void) {
		return NULL != wglGetCurrentContext();
	}

} // namespace SYS