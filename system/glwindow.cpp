/*
solution:	glib
project:	system
file:		glwindow.cpp
author:		cj
*/

#pragma comment(lib, "glu32.lib")

#include <Windows.h>
#include <gl\GLU.h>
#include "glwindow.h"

namespace SYS {

	void GLWindow::ResizeEvent(unsigned width, unsigned height) {
		base_t::ResizeEvent(width, height);

		glViewport(0, 0, width, height);

		const float aspect = (float)512 / 512;

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0f, aspect, 0.1f, 1000.0f);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}

	GLWindow::GLWindow(HINSTANCE instance, const TCHAR* title, unsigned width, unsigned height)
		: Window(instance, title, width, height),
		  _renderingContext(*this), // this is safe
		  _time(0.0f),
		  _frameTime(0.0f),
		  _frames(0),
		  _fps(0)
	{
		_renderingContext.Use();

		glClearColor(0.0f, 0.125f, 0.6f, 0.0f);

		glEnable(GL_DEPTH_TEST);
		glClearDepth(1.0f);

		ResizeEvent(width, height);
	}

	void GLWindow::operator()(float secsPassed) {
		_frameTime += secsPassed;
		if(1.0f <= _frameTime) {
			_fps = _frames;
			_frames = 0;
			_frameTime = 0.0f;
		}
		Render();
		_frames++;
		_renderingContext.Flip();
		
		if(1.0f <= secsPassed) {
			secsPassed = 1.0f;
		}

		_time += secsPassed;

		Move(secsPassed);
	}

} // namespace SYS