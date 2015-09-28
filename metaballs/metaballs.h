/*
solution:	glib
project:	metaballs
file:		metaballs.h
author:		cj
*/

#pragma once

#include <list>
#include "../common/pointer.h"
#include "../common/math3d.h"
#include "../prenderer/renderbuffer.h"
#include "../prenderer/framebuffer.h"
#include "../prenderer/program.h"
#include "../prenderer/posteffect.h"

struct Metaball {
	M::Vector2 position;
};

class Metaballs {
private:
	typedef GEN::Pointer<Metaball> ballPtr_t;
	typedef std::list<ballPtr_t>::iterator ballIt_t;

	std::list<ballPtr_t> _balls;

	// offscreen rendering
	enum BufferType {
		BT_POTENTIAL = 0,
		NUM_BUFFERS
	};

	GEN::Pointer<R::Texture> _texBuffers[NUM_BUFFERS];
	GEN::Pointer<R::Renderbuffer> _depthBuffer;

	R::Program _pass0;
public:
	void AddBall(ballPtr_t ball) { _balls.push_back(ball); }
};