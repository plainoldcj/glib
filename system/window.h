/*
solution:	glib
project:	system
file:		window.h
author:		cj
*/

#pragma once

#include <Windows.h>
#include "../common/uncopyable.h"

namespace SYS {

	struct MouseEvent {
		enum Type {
			TYPE_BUTTON_DOWN,
			TYPE_BUTTON_UP,
			TYPE_MOVE
		};
		enum Button {
			BUTTON_LEFT,
			BUTTON_NONE
		};

		Type type;
		Button reason;

		int flags;
		int x, y;
	};

	class Window : private GEN::Uncopyable {
		friend LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
	private:
		HWND _windowHandle;
		unsigned _width;
		unsigned _height;
	protected:
		virtual void ResizeEvent(unsigned width, unsigned height) { 
			_width = width;
			_height = height;
		}
		virtual void MouseEvent(const MouseEvent& event) { }
	public:
		Window(HINSTANCE instance, const TCHAR* title,
			   unsigned width, unsigned height);
		virtual ~Window(void);

		unsigned Width(void) const { return _width; }
		unsigned Height(void) const { return _height; }

		HWND GetNativeHandle(void) { return _windowHandle; }
	};

} // namespace SYS