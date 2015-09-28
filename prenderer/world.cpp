/*
solution:	glib
project:	prenderer
file:		world.cpp
author:		cj
*/

#include "../system/glx.h"
#include "../common/config.h"
#include "texallocs.h"
#include "program.h"
#include "fsquad.h"
#include "world.h"

namespace R {

	World::World(void) 
	{
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CW);
		glCullFace(GL_BACK);

		COM::Config& config = COM::Config::Instance();
		_width = config.Get("width", 512);
		_height = config.Get("height", 512);

		_worldMatrix = M::Mat4::Identity();
		
		for(int i = 0; i < NUM_BUFFERS; ++i) {
			_texBuffers[i] = GEN::Pointer<Texture>(new Texture(_width, _height, GL_RGB32F, GL_RGB));
			_framebuffer.Attach(Framebuffer::COLOR_BUFFER_0 + i, *_texBuffers[i]);
		}

		_depthBuffer = GEN::Pointer<Renderbuffer>(new Renderbuffer(Renderbuffer::DEPTH_BUFFER, _width, _height));
		_framebuffer.Attach(Framebuffer::DEPTH_BUFFER, *_depthBuffer);

		_deferred0.Attach("shader/vs_deferred0.txt", Shader::VERTEX_SHADER);
		_deferred0.Attach("shader/ps_deferred0.txt", Shader::PIXEL_SHADER);

		_pointlight.Attach("shader/vs_pointlight.txt", Shader::VERTEX_SHADER);
		_pointlight.Attach("shader/ps_pointlight.txt", Shader::PIXEL_SHADER);
	}

	void World::Draw(int flags, Mesh& mesh, Program& program) {
		glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);

		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, mesh.Vertices());

		if(DF_TEXCOORD & flags) {
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(2, GL_FLOAT, 0, mesh.TexCoords());
		}

		if(DF_TO_TANGENT & flags) {
			GLint loc;
			
			loc = program.GetAttributeLocation("attToTangentA0");
			glx.EnableVertexAttribArray(loc);
			glx.VertexAttribPointer(loc, 3, GL_FLOAT, false, sizeof(Vertex), &mesh.VertexData()->toTangent[0]);

			loc = program.GetAttributeLocation("attToTangentA1");
			glx.EnableVertexAttribArray(loc);
			glx.VertexAttribPointer(loc, 3, GL_FLOAT, false, sizeof(Vertex), &mesh.VertexData()->toTangent[1]);

			loc = program.GetAttributeLocation("attToTangentA2");
			glx.EnableVertexAttribArray(loc);
			glx.VertexAttribPointer(loc, 3, GL_FLOAT, false, sizeof(Vertex), &mesh.VertexData()->toTangent[2]);
		}

		if(DF_TO_OBJECT & flags) {
			GLint loc;

			loc = program.GetAttributeLocation("attToObjectA0");
			glx.EnableVertexAttribArray(loc);
			glx.VertexAttribPointer(loc, 3, GL_FLOAT, false, sizeof(Vertex), &mesh.VertexData()->toObject[0]);

			loc = program.GetAttributeLocation("attToObjectA1");
			glx.EnableVertexAttribArray(loc);
			glx.VertexAttribPointer(loc, 3, GL_FLOAT, false, sizeof(Vertex), &mesh.VertexData()->toObject[1]);
		}

		if((DF_TO_OBJECT | DF_NORMAL) & flags) {
			GLint loc = program.GetAttributeLocation("attNormal");
			glx.EnableVertexAttribArray(loc);
			glx.VertexAttribPointer(loc, 3, GL_FLOAT, false, sizeof(Vertex), &mesh.VertexData()->toObject[2]);
		}

		glDrawElements(GL_TRIANGLES, 3 * mesh.NumTriangles(),
			GL_UNSIGNED_INT, mesh.Triangles());

		glPopClientAttrib();
	}

	void World::DrawNormals(void) {
		UnbindFramebuffer();
		UnbindProgram();

		glLoadMatrixf(_worldMatrix.mat);

		glColor3f(1.0f, 1.0f, 1.0f);
		for(meshIt_t meshIt(_meshes.begin()); _meshes.end() != meshIt; ++meshIt) {
			Mesh& mesh = **meshIt;
			glBegin(GL_LINES);
			for(unsigned i = 0; i < mesh.NumTriangles() * 3; ++i) {
				M::Vector3 v0 = mesh.Vertices()[i];
				M::Vector3 v1 = mesh.VertexData()[i].toObject[2];
				glVertex3f(v0.x, v0.y, v0.z);
				glVertex3f(v1.x, v1.y, v1.z);
			}
			glEnd();
		}
	}

	void World::DrawBuffer(World::BufferType type) {
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);

		glPushAttrib(GL_ENABLE_BIT);
		glDisable(GL_DEPTH_TEST);

		UnbindFramebuffer();
		glx.UseProgram(0);
		_texBuffers[type]->Bind(0);

		glx.ActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, 0);
		glx.ActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, 0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glLoadIdentity();
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f,	 1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex2f( 1.0f,  1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex2f( 1.0f, -1.0f);
		glEnd();

		glPopAttrib();

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	}

	void World::AddMesh(World::meshPtr_t mesh) {
		if(!mesh->IsPrepared()) mesh->Prepare();
		_meshes.push_back(mesh);
	}

	void World::AddPostEffect(World::postEffectPtr_t postEffect) {
		_postEffects.push_back(postEffect);
	}

	/*
	void World::AddLight(World::lightPtr_t light) {
		_lights.push_back(light);
	}
	*/

	void World::AddBillboard(World::billboardPtr_t billboard) {
		_billboards.push_back(billboard);
	}

	void World::FillBuffers(void) {
		// ==================================
		// write shading information to offscreen buffer
		// ==================================

		glPushAttrib(GL_VIEWPORT_BIT);
		glViewport(0, 0, _width, _height);

		_framebuffer.Bind();
		_deferred0.Use();

		GLenum buffers[] = { 
			Framebuffer::COLOR_BUFFER_0, // position
			Framebuffer::COLOR_BUFFER_1, // normal
			Framebuffer::COLOR_BUFFER_2 // diffuse
		};
		glx.DrawBuffers(3, buffers);

		M::Matrix3 normalMatrix(_worldMatrix.m00, _worldMatrix.m01, _worldMatrix.m02,
								_worldMatrix.m10, _worldMatrix.m11, _worldMatrix.m12,
								_worldMatrix.m20, _worldMatrix.m21, _worldMatrix.m22);
		_deferred0.SetUniform("uniNormalMat", normalMatrix);

		glLoadMatrixf(_worldMatrix.mat);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		int drawFlags = DF_TEXCOORD | DF_TO_OBJECT;
		for(meshIt_t meshIt(_meshes.begin()); _meshes.end() != meshIt; ++meshIt) {
			Skin& skin = (*meshIt)->GetSkin();
			skin.Bind(BF_TEX_DIFFUSE | BF_TEX_NORMAL, _deferred0);
			Draw(drawFlags, **meshIt, _deferred0);
		}

		glPopAttrib();
	}

	void World::DrawPostEffects(void) {
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);

		glPushAttrib(GL_ENABLE_BIT);

		glDisable(GL_DEPTH_TEST);

		UnbindFramebuffer();

		for(postEffectIt_t postEffectIt(_postEffects.begin()); _postEffects.end() != postEffectIt; ++postEffectIt) {
			PostEffect& postEffect = **postEffectIt;

			Program& program = postEffect.GetProgram();
			program.Use();
			const char* names[] = {	"uniTexPosition", "uniTexNormal", "uniTexDiffuse" };
			for(int i = 0; i < NUM_BUFFERS; ++i) {
				program.SetUniform(names[i], i);
				_texBuffers[i]->Bind(i);
			}

			postEffect.Render(_worldMatrix);
		}

		glPopAttrib();

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	}

	/* DEPRECATED use posteffects instead */
	/*
	void World::DrawLights(void) {
		// ==================================
		// draw point lights as post effect
		// ==================================

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);

		glPushAttrib(GL_ENABLE_BIT);

		glDisable(GL_DEPTH_TEST);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		UnbindFramebuffer();
		_pointlight.Use();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();

		const char* names[] = {	"uniTexPosition", "uniTexNormal", "uniTexDiffuse" };
		for(int i = 0; i < NUM_BUFFERS; ++i) {
			_pointlight.SetUniform(names[i], i);
			_texBuffers[i]->Bind(i);
		}

		FSQuad& fsQuad = FSQuad::Instance();
		fsQuad.Begin();
		for(lightIt_t lightIt(_lights.begin()); _lights.end() != lightIt; ++lightIt) {
			Light& light = **lightIt;
			int flags = Light::BF_POSITION | Light::BF_COLOR | Light::BF_ATTENUATION;
			light.Bind(flags, _worldMatrix, _pointlight);
			fsQuad.Draw();
		}
		fsQuad.End();

		glPopAttrib();

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	}
	*/

	void World::DrawBillboards(void) {
		glPushAttrib(GL_CURRENT_BIT | GL_ENABLE_BIT);

		glDisable(GL_DEPTH_TEST);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		UnbindFramebuffer();
		UnbindProgram();

		M::Matrix4 orient(_worldMatrix.m00, _worldMatrix.m10, _worldMatrix.m20, 0.0f,
						  _worldMatrix.m01, _worldMatrix.m11, _worldMatrix.m21, 0.0f,
						  _worldMatrix.m02, _worldMatrix.m12, _worldMatrix.m22, 0.0f,
						  0.0f, 0.0f, 0.0f, 1.0f);
		M::Matrix4 world;
		for(billboardIt_t billboardIt(_billboards.begin()); _billboards.end() != billboardIt; ++billboardIt) {
			world = _worldMatrix * M::Mat4::Translate((*billboardIt)->GetPosition()) * orient;
			glLoadMatrixf(world.mat);

			Skin& skin = (*billboardIt)->GetSkin();
			skin.BindFFP(BF_TEX_DIFFUSE);
			const Color& col = skin.GetColor();
			glColor4f(col.r, col.g, col.b, 1.0f);
			Draw(DF_TEXCOORD, **billboardIt, _deferred0);
		}

		glPopAttrib();
	}

	void World::Draw(void) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		FillBuffers();
		//DrawBuffer(BT_DIFFUSE);
		DrawPostEffects();
		// DrawLights();
		DrawBillboards();
	}

} // namespace R