/*
solution:	glib
project:	cubemapping
file:		main.cpp
author:		cj
*/

#include <Windows.h>
#include "../filesystem/filesystem.h"
#include "../system/glwindow.h"
#include "../system/loop.h"

#include "cubemap.h"
#include "camera.h"

class Window : public SYS::GLWindow {
private:
	enum { WIDTH = 800, HEIGHT = 600 };

	CubeMap _cubeMap;
	Camera _camera;

	GLuint _program;
	rshader_t _vertexShader;
	rshader_t _pixelShader;

	rmd5Model_t _model;
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

	Window(void) 
		: base_t(NULL, TEXT("Cubemapping"), WIDTH, HEIGHT), 
		  _cubeMap("skybox/sky"),
		  _camera(WIDTH, HEIGHT) 
	{
		FS::Dump& dump = FS::Dump::Instance();

		R_CreateShader(&_vertexShader, GL_VERTEX_SHADER, "vertexshader.txt");
		R_CreateShader(&_pixelShader, GL_FRAGMENT_SHADER, "pixelshader.txt");

		_program = glx.CreateProgram();
		glx.AttachShader(_program, _vertexShader.shader);
		glx.AttachShader(_program, _pixelShader.shader);
		R_LinkProgram(_program);
		glx.UseProgram(_program);

		GLuint texLoc;
		texLoc = glx.GetUniformLocation(_program, "texNormal");
		glx.Uniform1i(texLoc, R_TEX_NORMAL);

		R_LoadModelMD5(&_model, "hunter/hunter.md5mesh");
		_model.meshes[0].texNormal = R_GetTexture(FS::Dump::Instance(), TYPE_TGA, "hunter/highres/hunterbody_local.tga");
		_model.meshes[1].texNormal = R_GetTexture(FS::Dump::Instance(), TYPE_TGA, "hunter/highres/hunterhead_local.tga");
	}

	void Render(void) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glLoadIdentity();
		glMultMatrixf(_camera.GetMatrix().mat);

		glx.UseProgram(0);
		_cubeMap.Render();

		M::Vector3 center = -_model.bbox.Center();

		M::Matrix4 world = M::Mat4::Translate(0.0f, 0.0f, -120.0f)
			* _camera.GetMatrix()
			* M::Mat4::RotateZ(90.0f)
			* M::Mat4::RotateY(90.0f)
			* M::Mat4::Translate(center.x, center.y, center.z);
		glLoadMatrixf(world.mat);

		glx.UseProgram(_program);
		_cubeMap.Bind(_program);

		M::Matrix4 normalMat = M::Mat4::RotateZ(90.0f) * M::Mat4::RotateY(90.0f);
		GLint normalMatLoc = glx.GetUniformLocation(_program, "normalMat");
		glx.UniformMatrix4fv(normalMatLoc, 1, GL_FALSE, normalMat.mat);

		M::Matrix4 inv = M::Mat4::RigidInverse(_camera.GetMatrix());
		M::Vector3 view = M::Normalize(M::Mat4::Transform(inv, M::Vector3(0.0f, 0.0f, -1.0f)));

		GLint viewLoc = glx.GetUniformLocation(_program, "view");
		glx.Uniform3f(viewLoc, view.x, view.y, view.z);

		R_DrawMeshMD5(&_model, _program, 0);
		R_DrawMeshMD5(&_model, _program, 1);
	}

	void Move(float secsPassed) {
	}
};

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd) {
	Window window;
	SYS::EnterLoop(window);

	return 0;
}