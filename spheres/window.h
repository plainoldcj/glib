/*
solution:	glib
project:	spheres
file:		window.h
author:		cj
*/

#pragma once

#include "../system/glwindow.h"
#include "camera.h"
#include "sphere.h"
#include "plane.h"

class Window : public SYS::GLWindow {
private:
	R::Camera _camera;

	Sphere _sphere;
	Plane _plane;
protected:
	void ResizeEvent(unsigned width, unsigned height) {
		base_t::ResizeEvent(width, height);
		_camera.SetScreenSize(width, height);
	}

	void MouseEvent(const SYS::MouseEvent& event) {
		if(SYS::MouseEvent::TYPE_BUTTON_DOWN == event.type) {
			if(SYS::MouseEvent::BUTTON_LEFT == event.reason) {
				_camera.StartDragging(event.x, event.y);
			}
		}
		if(SYS::MouseEvent::TYPE_BUTTON_UP == event.type) {
			if(SYS::MouseEvent::BUTTON_LEFT == event.reason) {
				_camera.StopDragging();
			}
		}
		if(SYS::MouseEvent::TYPE_MOVE == event.type) {
			_camera.Drag(event.x, event.y);
		}
	}
public:
	typedef SYS::GLWindow base_t;

	Window(int width, int height);

	void Render(void);
	void Move(float secsPassed);
};