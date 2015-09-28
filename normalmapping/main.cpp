/*
solution:	glib
project:	normalmapping
file:		main.cpp
author:		cj
*/

#include <Windows.h>
#include "../filesystem/filesystem.h"
#include "../common/math3d.h"
#include "../renderer/renderer.h"
#include "../system/glwindow.h"
#include "../system/loop.h"

class Window : public SYS::GLWindow {
private:
	float _angle;
	
	rtexture_t* _tex;
	
	GLuint _program;
	rshader_t _vertexShader;
	rshader_t _pixelShader;

	rmd5Model_t _model;

	M::Vector3 _lightPos;
public:
	Window(void) : SYS::GLWindow(0, TEXT("Normalmapping"), 800, 800) {
		_angle = 0.0f;

		R_Init();

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		R_CreateShader(&_vertexShader, GL_VERTEX_SHADER, "vertexshader.txt");
		R_CreateShader(&_pixelShader, GL_FRAGMENT_SHADER, "pixelshader.txt");

		_program = glx.CreateProgram();
		glx.AttachShader(_program, _vertexShader.shader);
		glx.AttachShader(_program, _pixelShader.shader);
		R_LinkProgram(_program);
		glx.UseProgram(_program);

		FS::Dump& dump = FS::Dump::Instance();

		/*
		R_LoadModelMD5(&_model, "hunter/hunter.md5mesh");

		_model.meshes[0].texDiffuse = R_GetTexture(dump, TYPE_TGA, "hunter/hunterbody.tga");
		_model.meshes[0].texNormal = R_GetTexture(dump, TYPE_TGA, "hunter/highres/hunterbody_local.tga");
		_model.meshes[0].texAdd = R_GetTexture(dump, TYPE_TGA, "hunter/hunterbody_add.tga");
		_model.meshes[0].texSpec = R_GetTexture(dump, TYPE_TGA, "hunter/hunterbody_s.tga");

		_model.meshes[1].texDiffuse = R_GetTexture(dump, TYPE_TGA, "hunter/hunterhead.tga");
		_model.meshes[1].texNormal = R_GetTexture(dump, TYPE_TGA, "hunter/hunterhead_local.tga");
		_model.meshes[1].texAdd = R_GetTexture(dump, TYPE_TGA, "hunter/hunterhead_add.tga");
		_model.meshes[1].texSpec = R_GetTexture(dump, TYPE_TGA, "hunter/hunterhead_s.tga");

		_model.meshes[2].texDiffuse = R_GetTexture(dump, TYPE_TGA, "rifle/rifle.tga");
		_model.meshes[2].texNormal = R_GetTexture(dump, TYPE_TGA, "rifle/rifle_local.tga");
		_model.meshes[2].texAdd = R_GetTexture(dump, TYPE_TGA, "rifle/rifle_add.tga");
		_model.meshes[2].texSpec = R_GetTexture(dump, TYPE_TGA, "rifle/rifle_s.tga");
		*/

		R_LoadModelMD5(&_model, "hellknight/hellknight.md5mesh");
		_model.meshes[0].texDiffuse = R_GetTexture(dump, TYPE_TGA, "hellknight/hellknight.tga");
		_model.meshes[0].texNormal = R_GetTexture(dump, TYPE_TGA, "hellknight/hellknight_local.tga");
		_model.meshes[0].texSpec = R_GetTexture(dump, TYPE_TGA, "hellknight/hellknight_s.tga");

		GLuint sampler;
		sampler = glx.GetUniformLocation(_program, "texDiffuse");
		glx.Uniform1i(sampler, 0);
		sampler = glx.GetUniformLocation(_program, "texNormal");
		glx.Uniform1i(sampler, 1);
		sampler = glx.GetUniformLocation(_program, "texAdd");
		glx.Uniform1i(sampler, 2);
		sampler = glx.GetUniformLocation(_program, "texSpec");
		glx.Uniform1i(sampler, 3);

		_lightPos = M::Vector3(-200.0f, 200.0f, 0.0f);
	}

	void Render(void) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindTexture(GL_TEXTURE_2D, 0);
		glx.UseProgram(0);

		glLoadIdentity();
		glTranslatef(_lightPos.x, _lightPos.y, _lightPos.z);

		const float s = 1.0f;
		glBegin(GL_QUADS);
		glVertex3f(-s, -s, 0.0f);
		glVertex3f(-s,  s, 0.0f);
		glVertex3f( s,  s, 0.0f);
		glVertex3f( s, -s, 0.0f);
		glEnd();

		glx.UseProgram(_program);

		M::Matrix4 world = M::Mat4::Translate(0.0f, -70.0f, -200.0f)
			* M::Mat4::RotateY(_angle)
			* M::Mat4::RotateZ(-90.0f)
			* M::Mat4::RotateY(-90.0f);
		glLoadMatrixf(world.mat);

		M::Matrix4 inv = M::Mat4::RigidInverse(world);
		M::Vector3 vec = M::Mat4::Transform(inv, _lightPos);
		int lightPosLoc = glx.GetUniformLocation(_program, "lightPos");
		glx.Uniform3f(lightPosLoc, vec.x, vec.y, vec.z);
		vec = M::Mat4::Transform(inv, M::Vector3(0.0f, 0.0f, 1.0f));
		int viewDirLoc = glx.GetUniformLocation(_program, "viewDir");
		glx.Uniform3f(viewDirLoc, vec.x, vec.y, vec.z);
		
		R_DrawMeshMD5(&_model, _program, 0);
		// R_DrawMeshMD5(&_model, _program, 1);
		// R_DrawMeshMD5(&_model, _program, 2);

		glx.UseProgram(0);

		/*R_DrawNormalsMD5(&_model, 0);
		R_DrawNormalsMD5(&_model, 1);*/
		/*R_DrawTangentSpaceMD5(&_model, 0);
		R_DrawTangentSpaceMD5(&_model, 1);*/
	}

	void Move(float secsPassed) {
		_angle += 20.0f * secsPassed;
	}
};

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd) {
	FS::Dump dump = FS::Dump::Instance();

	Window window;
	SYS::EnterLoop(window);

	return 0;
}