/*
solution:	glib
project:	prenderer
file:		pe_pointlights.h
author:		cj
*/

#pragma once

#include <list>
#include "../common/pointer.h"
#include "posteffect.h"
#include "light.h"
#include "program.h"

namespace R {
namespace PE {

	class Pointlights : public PostEffect {
	private:
		typedef GEN::Pointer<Light> lightPtr_t;
		typedef std::list<lightPtr_t>::iterator lightIt_t;

		std::list<lightPtr_t> _lights;
		Program _program;
	public:
		Pointlights(void);

		Program& GetProgram(void) { return _program; }
		void Render(const M::Matrix4& world);

		void AddLight(lightPtr_t light) { _lights.push_back(light); }
	};

} // namespace PE
} // namespace R