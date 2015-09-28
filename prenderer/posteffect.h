/*
solution:	glib
project:	prenderer
file:		posteffect.h
author:		cj
*/

#pragma once

namespace M { struct Matrix4; }

namespace R {

	class Program;

	class PostEffect {
	public:
		virtual Program& GetProgram(void) = 0;
		virtual void Render(const M::Matrix4& world) = 0;
	};

} // namespace R