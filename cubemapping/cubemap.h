/*
solution:	glib
project:	cubemapping
file:		cubemap.h
author:		cj
*/

#pragma once

#include "../renderer/renderer.h"

class CubeMap {
private:
	enum Face {
		RIGHT_FACE = 0,
		TOP_FACE,
		BACK_FACE,
		LEFT_FACE,
		BOTTOM_FACE,
		FRONT_FACE,
		NUM_FACES
	};
	rtexture_t* _textures[NUM_FACES];
public:
	CubeMap(const std::string& prefix);

	void Bind(GLuint program);
	void Render(void);
};