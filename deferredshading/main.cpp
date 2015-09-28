/*
solution:	glib
project:	deferredshading
file:		main.cpp
author:		cj
*/

#include <Windows.h>

#include "../common/config.h"
#include "../system/glx.h"
#include "../system/glwindow.h"
#include "../system/loop.h"
#include "../system/print.h"

#include "../prenderer/program.h"
#include "../prenderer/quad.h"
#include "../prenderer/world.h"
#include "../prenderer/camera.h"
#include "../prenderer/pe_pointlights.h"

class Light {
private:
	GEN::Pointer<R::Light> _rlight;
	M::Vector3 _position;
	GEN::Pointer<R::Quad> _quad;
	float _bounce;
	float _phase;
	float _time;
public:
	Light(void) {
		_time = 0.0f;

		// roll color
		float col[3];
		for(int i = 0; i < 3; ++i) col[i] = COM::Rand(0.5f, 1.0f);
		R::Color color(col[0], col[1], col[2]);

		// billboard
		_quad = GEN::Pointer<R::Quad>(new R::Quad(0.2f));

		GEN::Pointer<R::Texture> texture = R::TextureManager::Instance().Get("light.tga", FS::Dump::Instance());
		R::Skin& skin = _quad->GetSkin();
		skin.SetTexture(R::Skin::TL_DIFFUSE, texture);
		skin.SetColor(color);

		// light
		_rlight = GEN::Pointer<R::Light>(new R::Light());
		_rlight->SetConstantAttenuation(1.0f);
		_rlight->SetLinearAttenuation(0.2f);
		_rlight->SetQuadricAttenuation(0.2f);
		_rlight->SetDiffuseColor(color);

		float pos[2];
		for(int i = 0; i < 2; ++i) pos[i] = COM::Rand(-2.0f, 2.0f);
		_position = M::Vector3(pos[0], pos[1], 0.0f);

		_bounce = COM::Rand(1.0f, 5.0f);
		_phase = COM::Rand(1.0f, 2.0f);
	}

	GEN::Pointer<R::Quad> GetBillboard(void) { return _quad; }
	GEN::Pointer<R::Light> GetLight(void) { return _rlight; }

	void Move(float secsPassed) {
		_time += secsPassed;
		_position.z = COM::Abs(_bounce * sinf(_phase * _time));

		_quad->SetPosition(_position);
		_rlight->SetPosition(_position);
	}
};

class Window : public SYS::GLWindow {
private:
	GEN::Pointer<R::Mesh> _floor;
	R::Camera _camera;
	std::list<GEN::Pointer<Light> > _lights;
	int _numLights;

	int _printInfo;
protected:
	void ResizeEvent(unsigned width, unsigned height) {
		SYS::Window::ResizeEvent(width, height);
		glViewport(0, 0, width, height);
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
	Window(int width, int height)
		: SYS::GLWindow(NULL, TEXT("Deferred Shading"), width, height),
		  _camera(width, height)
	{
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

		COM::Config& config = COM::Config::Instance();
		_numLights = config.Get("numlights", 20);
		_printInfo = config.Get("printinfo", 0);

		GEN::Pointer<FS::File> file = FS::Dump::Instance().GetFile("logfile.txt");

		_floor = GEN::Pointer<R::Mesh>(new R::Quad(100.0f, 100.0f));

		GEN::Pointer<R::Texture> texDiffuse = R::TextureManager::Instance().Get("floor2.tga", FS::Dump::Instance());
		GEN::Pointer<R::Texture> texNormal = R::TextureManager::Instance().Get("normal.tga", FS::Dump::Instance());
		
		_floor->GetSkin().SetTexture(R::Skin::TL_DIFFUSE, texDiffuse);
		_floor->GetSkin().SetTexture(R::Skin::TL_NORMAL, texNormal);

		texNormal->Bind(0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		R::World& world = R::World::Instance();
		world.AddMesh(_floor);

		GEN::Pointer<R::PE::Pointlights> pointLights(new R::PE::Pointlights());

		for(int i = 0; i < _numLights; ++i) {
			GEN::Pointer<Light> light(new Light());
			world.AddBillboard(light->GetBillboard());
			pointLights->AddLight(light->GetLight());
			_lights.push_back(GEN::Pointer<Light>(light));
		}
		world.AddPostEffect(pointLights);
	}

	void Render(void) {
		R::World& world = R::World::Instance();

		M::Matrix4 worldMatrix = M::Mat4::Translate(0.0f, 0.0f, -6.0f) * _camera.GetMatrix();
		world.SetWorldMatrix(worldMatrix);

		world.Draw();

		if(_printInfo) {
			glx.ActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, 0);
			glColor3f(1.0f, 1.0f, 1.0f);

			SYS::Relocate(10, Height() - 20);
			SYS::Print("Num Lights: %d", _numLights);
			SYS::Print("Fps: %d", Fps());
		}
	}

	void Move(float secsPassed) {
		for(std::list<GEN::Pointer<Light> >::iterator lightIt(_lights.begin());
			_lights.end() != lightIt; ++lightIt)
		{
			(*lightIt)->Move(secsPassed);
		}
	}
};

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd) {
	try {
		COM::Config& config = COM::Config::Instance();
		int width = config.Get("width", 512);
		int height = config.Get("height", 512);

		Window window(width, height);
		SYS::EnterLoop(window);
	} catch(std::exception&) {
		MessageBox(NULL, TEXT("caught exception,\nview log for more information"),
				TEXT("Error"), MB_OK);
		return 1;
	}

	return 0;
}