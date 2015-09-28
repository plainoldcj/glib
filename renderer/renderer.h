/*
solution:	glib
project:	renderer
file:		renderer.h
author:		cj
*/

/*
this stuff is really DEPRECATED and totally
bad written. dont use it. use the prenderer
instead.
*/

#pragma once

#include <Windows.h>
#include <gl\GL.h>
#include "../common/common.h"
#include "../common/math3d.h"
#include "../system/extensions.h"

namespace FS { class Loader; }

int R_Init(void);

#define R_MAX_PATH 1024

void R_DrawVector(const M::Vector3& vec, float length);

/* textures */

typedef enum {
	R_TEX_DIFFUSE = 0,
	R_TEX_NORMAL,
	R_TEX_ADDITIVE,
	R_TEX_SPECULAR,

	R_TEX_CUBE_POS_X,
	R_TEX_CUBE_POS_Y,
	R_TEX_CUBE_POS_Z,
	R_TEX_CUBE_NEG_X,
	R_TEX_CUBE_NEG_Y,
	R_TEX_CUBE_NEG_Z,

	R_NUM_TEX_LEVELS
} rtexLevel_t;

typedef struct rtexture_s {
	struct rtexture_s* next;
	char filename[R_MAX_PATH];

	GLuint id;
	int width;
	int height;
	GLenum format;
	COM::byte_t* pixelData;
} rtexture_t;

typedef enum {
	TYPE_TGA = 0
} rtexType_t;

int R_LoadTexTGA(FS::Loader& loader, rtexture_t* tex, const char* filename);
rtexture_t* R_GetTexture(FS::Loader& loader, rtexType_t type, const char* filename);

/* models */

#define MAX_JOINT_NAME 1024

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

	/*
	these terms below are historically grown
	and are total bullshit. excuse me.
	*/

	// objectspace -> tangentspace
	M::Vector3 tangent;
	M::Vector3 bitangent;
	M::Vector3 normal;

	// tangentspace -> objectspace
	M::Vector3 invTangent;
	M::Vector3 invBitangent;
	M::Vector3 invNormal;
} rvert_t;

typedef struct {
	unsigned vertices[3];
} rtri_t;

typedef struct {
	int numVerts;
	int numTris;
	int numWeights;

	rmd5Vert_t* vertices;
	rmd5Tri_t* triangles;
	rmd5Weight_t* weights;

	rvert_t* vertexBuffer;
	rtri_t* indexBuffer;

	rtexture_t* texDiffuse;
	rtexture_t* texNormal;
	rtexture_t* texAdd;
	rtexture_t* texSpec;
} rmd5Mesh_t;

typedef struct {
	int numJoints;
	int numMeshes;

	rmd5Joint_t* joints;
	rmd5Mesh_t* meshes;

	M::BBox bbox;
} rmd5Model_t;

int R_LoadModelMD5(rmd5Model_t* model, const char* filename);
void R_DrawNormalsMD5(rmd5Model_t* model, int index);
void R_DrawTangentSpaceMD5(rmd5Model_t* model, int index);
void R_DrawMeshMD5(rmd5Model_t* model, GLuint program, int index);

/* shader */

typedef struct {
	bool valid;
	GLuint shader;
} rshader_t;

int R_CreateShader(rshader_t* shader, GLenum type, const char* filename);
void R_DeleteShader(rshader_t* shader);

int R_LinkProgram(GLuint program);

/* primitives */

void R_DrawCube(float size = 1.0f);