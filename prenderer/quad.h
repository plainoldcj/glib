/*
solution:	glib
project:	prenderer
file:		quad.h
author:		cj
*/

// simple quad, lying in the xy-plane.

#pragma once

#include "../common/math3d.h"
#include "mesh.h"

namespace R {

	class Quad : public Mesh {
	private:
		M::Vector3 _vertices[4];
		TexCoord _texCoords[4];
		Triangle _triangles[2];
	public:
		explicit Quad(float size = 1.0f, float texScale = 1.0f);
		Quad(const M::Vector2& lowerLeft, const M::Vector2& upperRight, float texScale = 1.0f);

		const M::Vector3* Vertices(void) const { return _vertices; }
		const TexCoord* TexCoords(void) const { return _texCoords; }
		const Triangle* Triangles(void) const { return _triangles; }

		unsigned NumTriangles(void) const { return 2; }
	};
	
} // namespace R