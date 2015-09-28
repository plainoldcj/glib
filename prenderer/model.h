/*
solution:	glib
project:	prenderer
file:		model.h
author:		cj
*/

#pragma once

/*
do not use this code, itll be rewritten
entirely.
*/

/*
the only supported model format is
doom3s .md5mesh, since those models
look really amazing and are (almost)
for free.
*/

#include "../common/math3d.h"
#include "../filesystem/filesystem.h"
#include "texture.h"
#include "program.h"

namespace R {

	#define MAX_JOINT_NAME 1024

	class Model {
	public:
		typedef struct {
			char name[MAX_JOINT_NAME];
			int parent;
			M::Vector3 pos;
			M::Quaternion orient;
		} rmd5Joint_t;

		typedef struct {
			int index;
			float s, t;
			int startWeight;
			int countWeight;
			int numTri;
		} rmd5Vert_t;

		typedef struct {
			int index;
			int vertices[3];
		} rmd5Tri_t;

		typedef struct {
			int index;
			int joint;
			float bias;
			M::Vector3 pos;
		} rmd5Weight_t;

		typedef struct {
			M::Vector3 pos;

			// objectspace -> tangentspace
			M::Vector3 toTangentA0;
			M::Vector3 toTangentA1;
			M::Vector3 toTangentA2;

			// tangentspace -> objectspace
			M::Vector3 toObjectA0;
			M::Vector3 toObjectA1;
			M::Vector3 normal; // toObjectA2
		} rvert_t;

		typedef struct {
			unsigned vertices[3];
		} rtri_t;

		enum DrawFlags {
			// attributes
			DF_NORMAL = (1 << 0),
			DF_TEXCOORD = (1 << 1),
			DF_TO_TANGENT = (1 << 2),
			DF_TO_OBJECT = (1 << 3),

			// textures
			DF_TEX_DIFFUSE = (1 << 4),
			DF_TEX_NORMAL = (1 << 5),
			DF_TEX_ADD = (1 << 6),
			DF_TEX_SPEC = (1 << 7)
		};

		typedef struct Mesh {
			friend class Model;
		private:
			int numVerts;
			int numTris;
			int numWeights;

			rmd5Vert_t* vertices;
			rmd5Tri_t* triangles;
			rmd5Weight_t* weights;

			rvert_t* vertexBuffer;
			rtri_t* indexBuffer;

			GEN::Pointer<Texture> texDiffuse;
			GEN::Pointer<Texture> texNormal;
			GEN::Pointer<Texture> texAdd;
			GEN::Pointer<Texture> texSpec;
		public:
			void SetDiffuseTexture(GEN::Pointer<Texture> texture) { texDiffuse = texture; }
			void SetNormalTexture(GEN::Pointer<Texture> texture) { texNormal = texture; }
			void SetAddTexture(GEN::Pointer<Texture> texture) { texAdd = texture; }
			void SetSpecTexture(GEN::Pointer<Texture> texture) { texSpec = texture; }

			void Draw(int flags, Program& program) const;
		} rmd5Mesh_t;

		typedef struct {
			char name[MAX_JOINT_NAME];
			int parent;
			int flags;
			int startIndex;
		} rmd5AnimJoint_t;

		typedef struct {
			M::Vector3 pos;
			M::Quaternion orient;
		} rmd5BaseJoint_t;

		typedef struct {
			float* comp;
		} rmd5Frame_t;

		typedef struct {
			int numFrames;
			int numJoints;
			int frameRate;
			int numAnimatedComponents;

			rmd5AnimJoint_t* joints;
			rmd5BaseJoint_t* baseFrame;
			rmd5Frame_t* frames;
		} rmd5Anim_t;
	public:
		int numJoints;
		int numMeshes;

		rmd5Joint_t* joints;
		rmd5Mesh_t* meshes;

		M::BBox bbox;

		void ParseMesh(GEN::Pointer<FS::File> file);
		void ParseAnim(GEN::Pointer<FS::File> file, rmd5Anim_t* anim);
		void ComputeBindPose(void);

		Mesh& GetMesh(int index) { return meshes[index]; }
	}; // class Model

} // namespace R