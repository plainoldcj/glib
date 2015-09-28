/*
solution:	glib
project:	spheres
file:		window.cpp
author:		cj
*/

#include "../math/matrix4.h"
#include "window.h"

Window::Window(int width, int height) 
	:	SYS::GLWindow(NULL, TEXT("Spheres"), width, height),
		_camera(width, height),
		_sphere(1.0f),
		_plane(10.0f)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // switch to wireframe

	_plane.SetPosition(M::Vector3(0.0f, -1.0f, 0.0f));
}

void Window::Render(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	M::Matrix4 world = M::Mat4::Translate(0.0f, 0.0f, -4.0f)
		* _camera.GetMatrix();
	glLoadMatrixf(world.mat);

	_sphere.RenderPreview();
	_plane.RenderPreview();
}

void Window::Move(float secsPassed) {
}