/*
solution:	glib
project:	system
file:		glwindow.h
author:		cj
*/

#pragma once

#include "../common/pointer.h"
#include "window.h"
#include "opengl.h"
#include "timer.h"

namespace SYS {

	class GLWindow : public Window {
	private:
		RenderingContext _renderingContext;
		float _time;
		
		float _frameTime;
		int _frames;
		int _fps;
	protected:
		virtual void ResizeEvent(unsigned width, unsigned height);
	public:
		typedef Window base_t;

		GLWindow(HINSTANCE instance, const TCHAR* title, unsigned width, unsigned height);

		float Time(void) const { return _time; }
		int Fps(void) const { return _fps; }

		void operator()(float secsPassed);

		virtual void Render(void) = 0;
		virtual void Move(float secsPassed) = 0;
	};

} // namespace SYS