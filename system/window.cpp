/*
solution:	glib
project:	system
file:		window.cpp
author:		cj
*/

#include "../common/common.h"
#include "window.h"

namespace SYS {

	void CommonMouseEvent(MouseEvent& event, WPARAM wParam, LPARAM lParam) {
		event.flags = wParam;
		event.x = LOWORD(lParam);
		event.y = HIWORD(lParam);
	}

	LRESULT CALLBACK WindowProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam) {
		Window* window = (Window*)GetWindowLongPtr(windowHandle, GWL_USERDATA);
		MouseEvent mouseEvent;

		switch(message) {
		case WM_SIZE:
			window->ResizeEvent(LOWORD(lParam), HIWORD(lParam));
			return 0;
		case WM_LBUTTONDOWN:
			mouseEvent.type = MouseEvent::TYPE_BUTTON_DOWN;
			mouseEvent.reason = MouseEvent::BUTTON_LEFT;
			CommonMouseEvent(mouseEvent, wParam, lParam);
			window->MouseEvent(mouseEvent);
			return 0;
		case WM_LBUTTONUP:
			mouseEvent.type = MouseEvent::TYPE_BUTTON_UP;
			mouseEvent.reason = MouseEvent::BUTTON_LEFT;
			CommonMouseEvent(mouseEvent, wParam, lParam);
			window->MouseEvent(mouseEvent);
			return 0;
		case WM_MOUSEMOVE:
			mouseEvent.type = MouseEvent::TYPE_MOVE;
			mouseEvent.reason = MouseEvent::BUTTON_NONE;
			CommonMouseEvent(mouseEvent, wParam, lParam);
			window->MouseEvent(mouseEvent);
			return 0;
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		}

		return DefWindowProc(windowHandle, message, wParam, lParam);
	}

	Window::Window(HINSTANCE instance, const TCHAR* title,
				   unsigned width, unsigned height) 
	{
		const TCHAR* className = TEXT("mainWindow");

		WNDCLASS windowClass;
		windowClass.style = CS_HREDRAW | CS_VREDRAW;
		windowClass.lpfnWndProc = WindowProc;
		windowClass.cbClsExtra = 0;
		windowClass.cbWndExtra = sizeof(Window*);
		windowClass.hInstance = instance;
		windowClass.hIcon = NULL;
		windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		windowClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
		windowClass.lpszMenuName = NULL;
		windowClass.lpszClassName = className;

		RegisterClass(&windowClass);

		DWORD style = WS_OVERLAPPEDWINDOW;
		RECT windowRect = { 0, 0, width, height };
		AdjustWindowRect(&windowRect, style, false);

		_windowHandle =
			CreateWindow(className, title, style, 0, 0,
						 windowRect.right - windowRect.left,
						 windowRect.bottom - windowRect.top,
						 NULL, NULL, instance, NULL);

		SetWindowLongPtr(_windowHandle, GWL_USERDATA, (LONG)this);

		UnregisterClass(className, instance);

		ShowWindow(_windowHandle, SW_SHOW);
		UpdateWindow(_windowHandle);
	}

	Window::~Window(void) {
		DestroyWindow(_windowHandle);
	}

} // namespace SYS