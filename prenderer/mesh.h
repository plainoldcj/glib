/*
solution:	glib
project:	prenderer
file:		mesh.h
author:		cj
*/

#pragma once

#include "../common/math3d.h"
#include "skin.h"

namespace R {

	struct TexCoord { 
		float s, t; 
	
		TexCoord(void) { }
		TexCoord(const TexCoord& other) : s(other.s), t(other.t) { }
		TexCoord(float s, float t) : s(s), t(t) { }
	};

	typedef unsigned index_t;

	struct Triangle {
		index_t vertices[3]; 
	
		Triangle(void) { }
		Triangle(const Triangle& other) {
			vertices[0] = other.vertices[0];
			vertices[1] = other.vertices[1];
			vertices[2] = other.vertices[2];
		}
		Triangle(index_t v0, index_t v1, index_t v2) {
			vertices[0] = v0;
			vertices[1] = v1;
			vertices[2] = v2;
		}
	};

	struct Vertex {
		int numTri;
		M::Vector3 toTangent[3];
		M::Vector3 toObject[3];
	};

	class Mesh {
	private:
		bool _prepared;
		Vertex* _vertices;
		Skin _skin;

		M::Vector3 _position;
	public:
		Mesh(void);
		~Mesh(void);

		bool IsPrepared(void) const { return _prepared; }

		void Prepare(void);
		Skin& GetSkin(void) { return _skin; }
		const Vertex* VertexData(void) const { return _vertices; }

		void SetPosition(const M::Vector3& position) { _position = position; }
		const M::Vector3& GetPosition(void) const { return _position; }

		virtual const M::Vector3* Vertices(void) const = 0;
		virtual const TexCoord* TexCoords(void) const = 0;
		virtual const Triangle* Triangles(void) const = 0;

		virtual unsigned VerticesStride(void) const { return 0; }
		virtual unsigned TexCoordsStride(void) const { return 0; }

		virtual unsigned NumTriangles(void) const = 0;
	};

} // namespace R