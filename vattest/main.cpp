/*
solution:	glib
project:	vattest
file:		main.cpp
author:		cj
*/

#include <Windows.h>

#include "../common/config.h"
#include "../system/glwindow.h"
#include "../system/loop.h"

#include "../renderutils/vat_model.h"

class Window : public SYS::GLWindow {
private:
	GEN::Pointer<R::UTIL::VATModel> _model;
	float _angle;
public:
	Window(int width, int height) : SYS::GLWindow(NULL, TEXT("VAT Test"), width, height) {
		_angle = 0.0f;

		FS::Dump& dump = FS::Dump::Instance();
		GEN::Pointer<FS::File> file = dump.GetFile("../resources/test.vat");
		_model = GEN::Pointer<R::UTIL::VATModel>(new R::UTIL::VATModel(file));
	}
	
	void RenderVAT(GEN::Pointer<R::UTIL::VATModel> model) {
		glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
		glEnable(GL_VERTEX_ARRAY);

		for(unsigned i = 0; i < model->NumMeshes(); ++i) {
			const R::Mesh& mesh = model->GetMesh(i);
			glVertexPointer(3, GL_FLOAT, mesh.VerticesStride(), mesh.Vertices());
			glDrawElements(GL_TRIANGLES, 3 * mesh.NumTriangles(), GL_UNSIGNED_INT, mesh.Triangles());
		}

		glPopClientAttrib();
	}

	void Render(void) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		M::Matrix4 world = M::Mat4::Translate(0.0f, 0.0f, -5.0f)
			* M::Mat4::RotateX(_angle)
			* M::Mat4::RotateY(_angle);
		glLoadMatrixf(world.mat);

		RenderVAT(_model);
	}

	void Move(float secsPassed) {
		_angle += 5.0f * secsPassed;
	}
};

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd) {
	try {
		COM::Config& config = COM::Config::Instance();
		int width = config.Get("width", 1024);
		int height = config.Get("height", 1024);

		Window window(width, height);
		SYS::EnterLoop(window);
	} catch(std::exception&) {
		MessageBox(NULL, TEXT("caught exception,\nview log for more information"),
				TEXT("Error"), MB_OK);
		return 1;
	}
	
	return 0;
}