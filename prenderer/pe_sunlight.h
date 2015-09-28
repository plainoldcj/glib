/*
solution:	glib
project:	prenderer
file:		pe_sunlight.h
author:		cj
*/

#pragma once

#include "../common/math3d.h"
#include "color.h"
#include "program.h"
#include "posteffect.h"

namespace R {
namespace PE {

	class Sunlight : public PostEffect {
	private:
		Program _program;

		M::Vector3 _direction;
		Color _color;
	public:
		Sunlight(void);

		Program& GetProgram(void) { return _program; }
		void Render(const M::Matrix4& world);

		void SetDirection(const M::Vector3& direction) { _direction = direction; }
		void SetColor(const Color& color) { _color = color; }
	};

} // namespace PE
} // namespace R