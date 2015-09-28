/*
solution:	glib
project:	prenderer
file:		world.h
author:		cj
*/

/*
this is the attempt to somewhat centralize the
rendering system. maybe ill drop it eventually.
*/

#pragma once

#include <list>
#include "../common/singleton.h"
#include "renderbuffer.h"
#include "framebuffer.h"
#include "program.h"
#include "texture.h"
#include "mesh.h"
#include "posteffect.h"
#include "light.h"
#include "quad.h"

namespace R {

	class Program;

	enum DrawFlags {
		DF_NORMAL = (1 << 0),
		DF_TEXCOORD = (1 << 1),
		DF_TO_TANGENT = (1 << 2),
		DF_TO_OBJECT = (1 << 3)
	};

	class World : public GEN::Singleton<World> {
		friend class GEN::Singleton<World>;
	private:
		typedef GEN::Pointer<Mesh> meshPtr_t;
		typedef GEN::Pointer<PostEffect> postEffectPtr_t;
		//typedef GEN::Pointer<Light> lightPtr_t;
		typedef GEN::Pointer<Quad> billboardPtr_t;
		typedef std::list<meshPtr_t>::iterator meshIt_t;
		typedef std::list<postEffectPtr_t>::iterator postEffectIt_t;
		//typedef std::list<lightPtr_t>::iterator lightIt_t;
		typedef std::list<billboardPtr_t>::iterator billboardIt_t;

		enum BufferType {
			BT_POSITION = 0,
			BT_NORMAL,
			BT_DIFFUSE,
			NUM_BUFFERS
		};

		int _width;
		int _height;
		
		GEN::Pointer<Texture> _texBuffers[NUM_BUFFERS];
		GEN::Pointer<Renderbuffer> _depthBuffer;
		Framebuffer _framebuffer;

		Program _deferred0;
		Program _pointlight;

		std::list<meshPtr_t> _meshes;
		std::list<postEffectPtr_t> _postEffects;
		//std::list<lightPtr_t> _lights;
		std::list<billboardPtr_t> _billboards;
		M::Matrix4 _worldMatrix;

		World(void);

		void Draw(int flags, Mesh& mesh, Program& program);
		void FillBuffers(void);
		void DrawPostEffects(void);
		//void DrawLights(void);

		void DrawNormals(void);
		void DrawBuffer(BufferType type);
		void DrawBillboards(void);
	public:
		void AddMesh(meshPtr_t mesh);
		void AddPostEffect(postEffectPtr_t postEffect);
		//void AddLight(lightPtr_t light);
		void AddBillboard(billboardPtr_t billboard);

		void SetWorldMatrix(const M::Matrix4& worldMatrix) { _worldMatrix = worldMatrix; }
		void Draw(void);
	};

} // namespace R