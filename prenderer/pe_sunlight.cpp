/*
solution:	glib
project:	prenderer
file:		pe_sunlight.cpp
author:		cj
*/

#include "texture.h"
#include "fsquad.h"
#include "pe_sunlight.h"

namespace R {
namespace PE {

	Sunlight::Sunlight(void) {
		_program.Attach("shader/vs_pointlight.txt", Shader::VERTEX_SHADER);
		_program.Attach("shader/ps_sunlight.txt", Shader::PIXEL_SHADER);

		_color = Color(1.0f, 1.0f, 1.0f);
		_direction = M::Vector3(0.0f, 0.0f, -1.0f);
	}

	void Sunlight::Render(const M::Matrix4& world) {
		M::Vector3 dir = M::Mat4::Transform(world, M::Negate(_direction));
		_program.SetUniform("uniLightDir", dir);
		_program.SetUniform("uniLightDiffuseCol", _color);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();

		FSQuad& fsQuad = FSQuad::Instance();
		fsQuad.Begin();
		fsQuad.Draw();
		fsQuad.End();
	}

} // namespace PE
} // namespace R