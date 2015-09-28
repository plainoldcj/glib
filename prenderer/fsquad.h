/*
solution:	glib
project:	prenderer
file:		fsquad.h
author:		cj
*/

/*
draw a fullscreen quad. projection matrix must be set
to identity. this class is optimized for static drawing,
so dont use R::Quad for this job.
*/

#pragma once

#include "../common/singleton.h"
#include "../system/glx.h"

namespace R {

	class FSQuad : public GEN::Singleton<FSQuad> {
		friend class GEN::Singleton<FSQuad>;
	private:
		enum Mode {
			M_VBO,
			M_DLIST
		};
		Mode mode;

		GLuint _dlist;

		typedef struct { float x, y, s, t; } vert_t;
		typedef struct { unsigned v0, v1, v2; } ind_t;

		GLint _texLoc;
		GLuint _vertexBuffer;
		GLuint _indexBuffer;

		FSQuad(void);
	public:
		void Begin(void);
		void Draw(void);
		void End(void);
	};

} // namespace R