/*
solution:	glib
project:	metaballs
file:		main.cpp
author:		cj
*/

#include <Windows.h>
#include "../common/config.h"
#include "../system/glwindow.h"
#include "../system/loop.h"
#include "../prenderer/quad.h"
#include "../prenderer/world.h"
#include "../prenderer/pe_sunlight.h"

#include "metaballs.h"

class Window : public SYS::GLWindow {
protected:
	void ResizeEvent(unsigned width, unsigned height) {
		SYS::Window::ResizeEvent(width, height);
		glViewport(0, 0, width, height);
	}
public:
	Window(unsigned width, unsigned height) : SYS::GLWindow(NULL, TEXT("Metaballs"), width, height)
	{
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.0f, width, 0.0f, height, 1.0f, -1.0f);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		R::TextureManager& texManager = R::TextureManager::Instance();
		R::World& world = R::World::Instance();

		GEN::Pointer<R::Mesh> bgQuad(new R::Quad(M::Vector2(0.0f, 0.0f), M::Vector2(width, height)));
		R::Skin& skin = bgQuad->GetSkin();
		skin.SetTexture(R::Skin::TL_DIFFUSE, texManager.Get("res/background.tga"));
		skin.SetTexture(R::Skin::TL_NORMAL, texManager.Get("res/normal.tga"));

		world.AddMesh(bgQuad);

		///*
		GEN::Pointer<R::PE::Sunlight> sunlight(new R::PE::Sunlight());
		sunlight->SetColor(R::Color(1.0f, 1.0f, 1.0f));
		sunlight->SetDirection(M::Vector3(0.0f, 0.0f, -1.0f));
		world.AddPostEffect(sunlight);
		//*/

		/*
		GEN::Pointer<R::PE::Pointlights> pointlights(new R::PE::Pointlights());
		GEN::Pointer<R::Light> light(new R::Light());
		light->SetDiffuseColor(R::Color(1.0f, 0.0f, 0.0f));
		light->SetPosition(M::Vector3(width / 2, height / 2, 400.0f));
		pointlights->AddLight(light);
		world.AddPostEffect(pointlights);
		*/

		ResizeEvent(width, height);
	}

	void Render(void) {
		R::World& world = R::World::Instance();
		world.Draw();
	}

	void Move(float secsPassed) {
	}
};

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd) {
	try {
		COM::Config& config = COM::Config::Instance();
		unsigned width = config.Get("width", 800);
		unsigned height = config.Get("height", 600);
		Window window(width, height);
		SYS::EnterLoop(window);
	} catch(std::exception& err) {
		MessageBox(NULL, TEXT("caught exception,\nview log for more information"),
				TEXT("Error"), MB_OK);
		return 1;
	}
	return 0;
}