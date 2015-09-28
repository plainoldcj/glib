/*
solution:	glib
project:	prenderer
file:		mesh.cpp
author:		cj
*/

#include "mesh.h"

namespace R {

	Mesh::Mesh(void) : _prepared(false), _vertices(NULL), _position(M::Vector3::Zero) { }

	Mesh::~Mesh(void) {
		if(NULL != _vertices) {
			delete[] _vertices;
		}
	}

	// computes toTangent*/toObject* stuff
	void Mesh::Prepare(void) {
		if(_prepared) return;

		unsigned numVertices = 3 * NumTriangles();
		_vertices = new Vertex[numVertices];

		for(unsigned i = 0; i < numVertices; ++i) {
			_vertices[i].numTri = 0;
			_vertices[i].toObject[2] = M::Vector3::Zero;
		}

		for(unsigned i = 0; i < NumTriangles(); ++i) {
			const Triangle* triangle = &Triangles()[i];
			for(int j = 0; j < 3; ++j) {
				int x0 = triangle->vertices[j];
				int x1 = triangle->vertices[(j + 1) % 3];
				int x2 = triangle->vertices[(j + 2) % 3];

				M::Vector3 a0 = Vertices()[x0];
				M::Vector3 a1 = Vertices()[x1];
				M::Vector3 a2 = Vertices()[x2];

				M::Vector3 v1 = Vertices()[x1] - Vertices()[x0];
				M::Vector3 v2 = Vertices()[x2] - Vertices()[x0];

				float d1_0 = TexCoords()[x1].s - TexCoords()[x0].s;
				float d1_1 = TexCoords()[x1].t - TexCoords()[x0].t;

				float d2_0 = TexCoords()[x2].s - TexCoords()[x0].s;
				float d2_1 = TexCoords()[x2].t - TexCoords()[x0].t;

				float det = d1_0 * d2_1 - d2_0 * d1_1;
				_vertices[x0].toObject[0] = (d2_1 * v1 - d1_1 * v2) / det;
				_vertices[x0].toObject[1] = (-d2_0 * v1 + d1_0 * v2) / det;
				_vertices[x0].toObject[2] += M::Cross(v2, v1);
				_vertices[x0].numTri++;
			}
		}

		for(unsigned i = 0; i < numVertices; ++i) {
			_vertices[i].toObject[2] /= (float)_vertices[i].numTri;

			for(int j = 0; j < 3; ++j) {
				_vertices[i].toObject[j].Normalize();
			}

			M::Vector3 A0 = _vertices[i].toObject[0];
			M::Vector3 A1 = _vertices[i].toObject[1];
			M::Vector3 A2 = _vertices[i].toObject[2];

			M::Matrix3 mat = M::Mat3::FromColumns(A0, A1, A2);
			M::Matrix3 inv = M::Mat3::Inverse(mat);
			M::Mat3::ToColumns(inv, A0, A1, A2);

			_vertices[i].toTangent[0] = A0;
			_vertices[i].toTangent[1] = A1;
			_vertices[i].toTangent[2] = A2;
		}

		_prepared = true;
	}

} // namespace R