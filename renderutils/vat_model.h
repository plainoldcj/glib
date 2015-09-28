/*
solution:	glib
project:	renderutils
file:		vat_model.h
author:		cj
*/

#pragma once 

#include <vector>
#include "../common/pointer.h"
#include "../filesystem/filesystem.h"
#include "../prenderer/mesh.h"

namespace R {
namespace UTIL {

	class VATModel {
	private:
		struct Vertex {
			M::Vector3 pos;
			TexCoord texCoord;
		};
		struct VATMesh : Mesh {
			std::vector<Vertex> vertices;
			std::vector<Triangle> triangles;

			const M::Vector3* Vertices(void) const { return &vertices[0].pos; }
			const TexCoord* TexCoords(void) const { return NULL; }
			const Triangle* Triangles(void) const { return &triangles[0]; }

			unsigned VerticesStride(void) const { return sizeof(Vertex); }

			unsigned NumTriangles(void) const { return triangles.size(); }
		};
		std::vector<GEN::Pointer<VATMesh> > _meshes;
	public:
		VATModel(GEN::Pointer<FS::File> file);

		unsigned NumMeshes(void) const { return _meshes.size(); }
		const Mesh& GetMesh(unsigned index) const { return *_meshes[index]; }
	};

} // namespace UTIL
} // namespace R