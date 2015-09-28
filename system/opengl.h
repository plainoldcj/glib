/*
solution:	glib
project:	system
file:		opengl.h
author:		cj
*/

#pragma once

#include <exception>
#include <Windows.h>
#include "../common/uncopyable.h"
#include "extensions.h"

namespace SYS {

	class Window;

	struct PixelFormatException : std::exception {
		const char* what(void) const {
			return "setting pixel format failed";
		}
	};

	class DeviceContext : private GEN::Uncopyable {
	protected:
		HWND _windowHandle;
		HDC _contextHandle;
	public:
		DeviceContext(Window& window);
		virtual ~DeviceContext(void);

		HDC GetNativeHandle(void) { return _contextHandle; }
	};

	struct GLDeviceContext : DeviceContext {
		GLDeviceContext(Window& window);
	};

	class RenderingContext : private GEN::Uncopyable {
	private:
		GLDeviceContext _deviceContext;
		HGLRC _contextHandle;
		Extensions _extensions;
		bool _initialized;

		void PrepareFont(void);
		void LoadExtensions(void);
	public:
		RenderingContext(Window& window);
		~RenderingContext(void);

		void Use(void);
		void Flip(void);
	};

	bool IsRenderingContextActive(void);

} // namespace SYS