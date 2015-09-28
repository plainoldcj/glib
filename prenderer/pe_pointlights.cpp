/*
solution:	glib
project:	prenderer
file:		pe_pointlights.cpp
author:		cj
*/

#include "fsquad.h"
#include "pe_pointlights.h"

namespace R {
namespace PE {

	Pointlights::Pointlights(void) {
		_program.Attach("shader/vs_pointlight.txt", Shader::VERTEX_SHADER);
		_program.Attach("shader/ps_pointlight.txt", Shader::PIXEL_SHADER);
	}

	void Pointlights::Render(const M::Matrix4& world) {
		glPushAttrib(GL_ENABLE_BIT);
		
		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();

		FSQuad& fsQuad = FSQuad::Instance();
		fsQuad.Begin();
		for(lightIt_t lightIt(_lights.begin()); _lights.end() != lightIt; ++lightIt) {
			Light& light = **lightIt;
			int flags = Light::BF_POSITION | Light::BF_COLOR | Light::BF_ATTENUATION;
			light.Bind(flags, world, _program);
			fsQuad.Draw();
		}
		fsQuad.End();

		glPopAttrib();
	}

} // namespace PE
} // namespace R