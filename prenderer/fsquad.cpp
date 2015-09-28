/*
solution:	glib
project:	prenderer
file:		fsquad.cpp
author:		cj
*/

#include "../common/config.h"
#include "../common/common.h"
#include "fsquad.h"

#define BUFFER_OFFSET(x) ((GLubyte*)0 + x)

namespace R {

	FSQuad::FSQuad(void) {
		COM::Config& config = COM::Config::Instance();
		std::string cvar = config.Get("fsquadmode", "vbo");
		if(cvar == "vbo") mode = M_VBO;
		else if(cvar == "dlist") mode = M_DLIST;
		else {
			COM::log << "FSQuad::FSQuad: unknown mode \"" << cvar <<
				"\". will use vbo." << std::endl;
			mode = M_VBO;
		}

		if(M_VBO == mode) {
			static const vert_t vertices[] = {
				{ -1.0f, -1.0f, 0.0f, 0.0f },
				{ -1.0f,  1.0f, 0.0f, 1.0f },
				{  1.0f,  1.0f, 1.0f, 1.0f },
				{  1.0f, -1.0f, 1.0f, 0.0f }
			};
			static const ind_t indices[] = {
				{ 0, 1, 2 }, { 0, 2, 3 }
			};

			glx.GenBuffers(1, &_vertexBuffer);
			glx.BindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
			glx.BufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			glx.GenBuffers(1, &_indexBuffer);
			glx.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);
			glx.BufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

			glx.BindBuffer(GL_ARRAY_BUFFER, 0);
			glx.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}

		if(M_DLIST == mode) {
			_dlist = glGenLists(1);
			glNewList(_dlist, GL_COMPILE);
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, -1.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f,	 1.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex2f( 1.0f,  1.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex2f( 1.0f, -1.0f);
			glEnd();
			glEndList();
		}
	}

	void FSQuad::Begin(void) {
		if(M_VBO == mode) {
			glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);

			glx.BindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
			glx.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);
			glVertexPointer(2, GL_FLOAT, sizeof(vert_t), BUFFER_OFFSET(0));
			glTexCoordPointer(2, GL_FLOAT, sizeof(vert_t), BUFFER_OFFSET(2 * sizeof(float)));
		}
	}

	void FSQuad::Draw(void) {
		if(M_VBO == mode) {
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
		}
		if(M_DLIST == mode) {
			glCallList(_dlist);
		}
	}

	void FSQuad::End(void) {
		if(M_VBO == mode) {
			glx.BindBuffer(GL_ARRAY_BUFFER, 0);
			glx.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glPopClientAttrib();
		}
	}

} // namespace R