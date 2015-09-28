/*
solution:	glib
project:	application
file:		main.cpp
author:		cj
*/

#include <Windows.h>
#include "../filesystem/filesystem.h"
#include "../common/math3d.h"
#include "../system/glwindow.h"
#include "../system/loop.h"

#include "../prenderer/texture.h"
#include "../prenderer/texallocs.h"
#include "../prenderer/program.h"
#include "../prenderer/model.h"

class Window : public SYS::GLWindow {
private:
	R::Program _program;
	R::Model _model;
public:
	Window(void) : SYS::GLWindow(0, TEXT("Application Window"), 800, 800) {
		_program.Attach("vertexshader.txt", R::Shader::VERTEX_SHADER);
		_program.Attach("pixelshader.txt", R::Shader::PIXEL_SHADER);

		_model.ParseMesh(FS::Disk::Instance().GetFile("hunter/hunter.md5mesh"));
		_model.ComputeBindPose();

		R::rmd5Anim_t anim;
		_model.ParseAnim(FS::Disk::Instance().GetFile("hunter/anim/workingidle.md5anim"), &anim);

		_model.GetMesh(0).SetDiffuseTexture(R::TextureManager::Instance().Get("hunter/hunterbody.tga"));
		_model.GetMesh(1).SetDiffuseTexture(R::TextureManager::Instance().Get("hunter/hunterhead.tga"));
	}

	void Render(void) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		M::Vector3 center = -_model.bbox.Center();

		M::Matrix4 world = M::Mat4::Translate(0.0f, 0.0f, -200.0f)
			* M::Mat4::RotateY(Time() * 2.0f)
			* M::Mat4::RotateZ(90.0f)
			* M::Mat4::RotateY(90.0f)
			* M::Mat4::Translate(center.x, center.y, center.z);
		glLoadMatrixf(world.mat);

		int flags = R::DF_TEXCOORD | R::DF_TEX_DIFFUSE;
		for(int i = 0; i < 2; ++i) {
			R::Mesh& mesh = _model.GetMesh(i);
			mesh.Draw(flags, _program);
		}
	}

	void Move(float secsPassed) {
	}
};

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd) {
	try {
		Window window;
		SYS::EnterLoop(window);
	} catch(std::exception&) {
		MessageBox(NULL, TEXT("caught exception,\nview logfile for more information"), TEXT("Error"), MB_OK);
	}

	return 0;
}