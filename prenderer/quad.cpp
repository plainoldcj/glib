/*
solution:	glib
project:	prenderer
file:		quad.cpp
author:		cj
*/

#include "quad.h"

namespace R {

	Quad::Quad(float size, float texScale) {
		_vertices[0] = M::Vector3(-size, -size, 0.0f);
		_vertices[1] = M::Vector3(-size,  size, 0.0f);
		_vertices[2] = M::Vector3( size,  size, 0.0f);
		_vertices[3] = M::Vector3( size, -size, 0.0f);

		_texCoords[0] = TexCoord(0.0f, 0.0f);
		_texCoords[1] = TexCoord(0.0f, texScale);
		_texCoords[2] = TexCoord(texScale, texScale);
		_texCoords[3] = TexCoord(texScale, 0.0f);

		_triangles[0] = Triangle(0, 1, 2);
		_triangles[1] = Triangle(0, 2, 3);
	}

	Quad::Quad(const M::Vector2& lowerLeft, const M::Vector2& upperRight, float texScale) {
		_vertices[0] = M::Vector3(lowerLeft.x, lowerLeft.y, 0.0f);
		_vertices[1] = M::Vector3(lowerLeft.x, upperRight.y, 0.0f);
		_vertices[2] = M::Vector3(upperRight.x, upperRight.y, 0.0f);
		_vertices[3] = M::Vector3(upperRight.x, lowerLeft.y, 0.0f);

		_texCoords[0] = TexCoord(0.0f, 0.0f);
		_texCoords[1] = TexCoord(0.0f, texScale);
		_texCoords[2] = TexCoord(texScale, texScale);
		_texCoords[3] = TexCoord(texScale, 0.0f);

		_triangles[0] = Triangle(0, 1, 2);
		_triangles[1] = Triangle(0, 2, 3);
	}

} // namespace R