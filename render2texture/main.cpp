/*
solution:	glib
project:	render2texture
file:		main.cpp
author:		cj
*/

#include <Windows.h>

#include "../system/glwindow.h"
#include "../system/loop.h"
#include "../filesystem/filesystem.h"
#include "../prenderer/model.h"
#include "../prenderer/texallocs.h"
#include "../prenderer/texture.h"
#include "../prenderer/framebuffer.h"
#include "../prenderer/renderbuffer.h"
#include "../common/math3d.h"

#define TEX_SIZE 1024

class Window : public SYS::GLWindow {
private:
	float _angle;
	R::Model _model;
	R::Program _program;

	R::Texture _texDiffuse;
	R::Texture _texNormal;
	R::Texture _texSpec;
	R::Texture _texFinal;

	R::Framebuffer _framebuffer;
	R::Renderbuffer _depthBuffer;
public:
	Window(void) 
		: SYS::GLWindow(NULL, TEXT("Render To Texture"), 800, 600),
		  _texDiffuse(TEX_SIZE, TEX_SIZE, GL_RGBA),
		  _texNormal(TEX_SIZE, TEX_SIZE, GL_RGBA),
		  _texSpec(TEX_SIZE, TEX_SIZE, GL_RGBA),
		  _texFinal(TEX_SIZE, TEX_SIZE, GL_RGBA),
		  _depthBuffer(R::Renderbuffer::DEPTH_BUFFER, TEX_SIZE, TEX_SIZE)
	{
		_angle = 0.0f;

		_program.Attach("vertexshader.txt", R::Shader::VERTEX_SHADER);
		_program.Attach("pixelshader.txt", R::Shader::PIXEL_SHADER);

		R::TextureManager& texManager = R::TextureManager::Instance();
		FS::Dump& dump = FS::Dump::Instance();

		_model.ParseMesh(dump.GetFile("hunter/hunter.md5mesh"));
		_model.ComputeBindPose();

		R::Model::Mesh& body = _model.GetMesh(0);
		body.SetDiffuseTexture(texManager.Get("hunter/hunterbody.tga", dump));
		body.SetNormalTexture(texManager.Get("hunter/highres/hunterbody_local.tga", dump));
		body.SetAddTexture(texManager.Get("hunter/hunterbody_add.tga", dump));
		body.SetSpecTexture(texManager.Get("hunter/hunterbody_s.tga", dump));

		R::Model::Mesh& head = _model.GetMesh(1);
		head.SetDiffuseTexture(texManager.Get("hunter/hunterhead.tga", dump));
		head.SetNormalTexture(texManager.Get("hunter/hunterhead_local.tga", dump));
		head.SetAddTexture(texManager.Get("hunter/hunterhead_add.tga", dump));
		head.SetSpecTexture(texManager.Get("hunter/hunterhead_s.tga", dump));

		_framebuffer.Attach(R::Framebuffer::COLOR_BUFFER_0, _texDiffuse);
		_framebuffer.Attach(R::Framebuffer::COLOR_BUFFER_1, _texNormal);
		_framebuffer.Attach(R::Framebuffer::COLOR_BUFFER_2, _texSpec);
		_framebuffer.Attach(R::Framebuffer::COLOR_BUFFER_3, _texFinal);
		_framebuffer.Attach(R::Framebuffer::DEPTH_BUFFER, _depthBuffer);
	}

	void RenderToTextures(void) {
		_framebuffer.Bind();
		glPushAttrib(GL_VIEWPORT_BIT);
		glViewport(0, 0, TEX_SIZE, TEX_SIZE);
		GLenum buffers[] = {
			R::Framebuffer::COLOR_BUFFER_0,
			R::Framebuffer::COLOR_BUFFER_1,
			R::Framebuffer::COLOR_BUFFER_2,
			R::Framebuffer::COLOR_BUFFER_3
		};
		glx.DrawBuffers(4, buffers);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		M::Matrix4 world = M::Mat4::Translate(0.0f, -79.0f, -30.0f)
			* M::Mat4::RotateY(_angle)
			* M::Mat4::RotateZ(-90.0f)
			* M::Mat4::RotateY(-90.0f);
		glLoadMatrixf(world.mat);

		_program.Use();
		M::Matrix4 inv = M::Mat4::RigidInverse(world);
		M::Vector3 lightPos = M::Vector3(-50.0f, 20.0f, 0.0f);
		_program.SetUniform("uniLightPos", M::Mat4::Transform(inv, lightPos));
		M::Vector3 viewDir = M::Vector3(0.0f, 0.0f, 1.0f);
		_program.SetUniform("uniViewDir", M::Mat4::Transform(inv, viewDir));

		int flags = R::Model::DF_TEXCOORD | R::Model::DF_NORMAL | R::Model::DF_TO_TANGENT |
			R::Model::DF_TEX_DIFFUSE | R::Model::DF_TEX_NORMAL | R::Model::DF_TEX_ADD | R::Model::DF_TEX_SPEC;
		for(int i = 0; i < 2; ++i) {
			_model.GetMesh(i).Draw(flags, _program);
		}

		glPopAttrib();
		R::UnbindFramebuffer();
	}

	void RenderQuad(void) {
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glOrtho(0.0f, Width(), 0.0f, Height(), -1.0f, 1.0f);
		glMatrixMode(GL_MODELVIEW);

		const float hw = Width() / 2.0f;
		const float hh = Height() / 2.0f;
		const float w = Width();
		const float h = Height();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();

		glx.UseProgram(0);
		glx.ActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);

		// lower left
		_texSpec.Bind(0);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(0, 0);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(hw, 0);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(hw, hh);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(0, hh);
		glEnd();

		// upper left
		_texDiffuse.Bind(0);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(0, hh);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(hw, hh);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(hw, h);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(0, h);
		glEnd();

		// lower right
		_texFinal.Bind(0);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(hw, 0);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(w, 0);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(w, hh);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(hw, hh);
		glEnd();

		// upper right
		_texNormal.Bind(0);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(hw, hh);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(w, hh);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(w, h);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(hw, h);
		glEnd();

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	}

	void Render(void) {
		RenderToTextures();
		RenderQuad();
	}

	void Move(float secsPassed) {
		_angle += secsPassed * 10.0f;
	}
};

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd) {
	try {
		Window window;
		SYS::EnterLoop(window);
	} catch(std::exception&) {
		MessageBox(NULL, TEXT("caught exception,\nview log for more information"),
				TEXT("Error"), MB_OK);
		return 1;
	}

	return 0;
}