/*
solution:	glib
project:	prenderer
file:		model.cpp
author:		cj
*/

#include "model.h"

#define LINE_BUFFER_SIZE 2048

namespace R {

	void ComputeW(M::Quaternion& quat) {
		float t = 1.0f
			- (quat.x * quat.x)
			- (quat.y * quat.y)
			- (quat.z * quat.z);
		if(t < 0.0f) quat.w = 0.0f;
		else quat.w = -sqrt(t);
	}

	void Model::ParseMesh(GEN::Pointer<FS::File> file) {
		bool parsed;
		char lineBuffer[LINE_BUFFER_SIZE];
		int numRead;
		rmd5Joint_t* joint;
		rmd5Mesh_t* mesh;
		rmd5Vert_t* vert;
		rtri_t* ind;
		rmd5Tri_t* tri;
		rmd5Weight_t* weight;
		ctoken_t *tokens, *token;
		M::Vector3 pos, weightPos;
		int i;

		numJoints = numMeshes = -1;
	
		while(!file->Eof()) {
			memset(lineBuffer, 0, LINE_BUFFER_SIZE);
			file->Gets(lineBuffer, LINE_BUFFER_SIZE);
		
			COM_Tokenize(&tokens, lineBuffer);
			token = tokens;

			if(NULL == token) continue;

			if(!strcmp(token->string, "numJoints")) {
				token = token->next;
				numJoints = token->i;

				joints = (rmd5Joint_t*)malloc(sizeof(rmd5Joint_t) * numJoints);
				if(!joints) {
					COM::log << "out of memory reading " << file->Name() << std::endl;
					throw std::bad_alloc();
				}
				joint = joints;

				continue;
			}

			if(!strcmp(token->string, "numMeshes")) {
				token = token->next;
				numMeshes = token->i;

				meshes = (rmd5Mesh_t*)malloc(sizeof(rmd5Mesh_t) * numMeshes);
				if(!meshes) {
					COM::log << "out of memory reading " << file->Name() << std::endl;
					throw std::bad_alloc();
				}
				mesh = meshes;

				continue;
			}

			if(!strcmp(token->string, "joints")) {
				COM_FreeTokens(tokens);
				tokens = NULL;

				while(!file->Eof()) {
					memset(lineBuffer, 0, LINE_BUFFER_SIZE);
					file->Gets(lineBuffer, LINE_BUFFER_SIZE);
					if('}' == lineBuffer[0]) break;

					COM_Tokenize(&tokens, lineBuffer);
					token = tokens;

					strcpy(joint->name, token->string);
					token = token->next;

					joint->parent = token->i;
					token = token->next;

					token = token->next;
					joint->pos.x = token->f;
					token = token->next;
					joint->pos.y = token->f;
					token = token->next;
					joint->pos.z = token->f;
					token = token->next;
					token = token->next;

					token = token->next;
					joint->orient.x = token->f;
					token = token->next;
					joint->orient.y = token->f;
					token = token->next;
					joint->orient.z = token->f;
					token = token->next;
					token = token->next;

					ComputeW(joint->orient);

					joint++;

					COM_FreeTokens(tokens);
				}
				continue;
			} // reading joints

			if(!strcmp(token->string, "mesh")) {
				COM_FreeTokens(tokens);
				tokens = NULL;

				new(mesh) rmd5Mesh_t;

				while(!file->Eof()) {
					memset(lineBuffer, 0, LINE_BUFFER_SIZE);
					file->Gets(lineBuffer, LINE_BUFFER_SIZE);
					if('}' == lineBuffer[0]) break;

					COM_Tokenize(&tokens, lineBuffer);
					token = tokens;

					if(!strcmp(token->string, "numverts")) {
						token = token->next;
						mesh->numVerts = token->i;

						mesh->vertices = (rmd5Vert_t*)malloc(sizeof(rmd5Vert_t) * mesh->numVerts);
						mesh->vertexBuffer = (rvert_t*)malloc(sizeof(rvert_t) * mesh->numVerts);
					
						if(!mesh->vertices || !mesh->vertexBuffer) {
							COM::log << "out of memory reading " << file->Name() << std::endl;
							throw std::bad_alloc();
						}
						vert = mesh->vertices;

						continue;
					}

					if(!strcmp(token->string, "numtris")) {
						token = token->next;
						mesh->numTris = token->i;

						mesh->triangles = (rmd5Tri_t*)malloc(sizeof(rmd5Tri_t) * mesh->numTris);
						mesh->indexBuffer = (rtri_t*)malloc(sizeof(rtri_t) * mesh->numTris);
					
						if(!mesh->triangles || !mesh->indexBuffer) {
							COM::log << "out of memory reading " << file->Name() << std::endl;
							throw std::bad_alloc();
						}
						tri = mesh->triangles;
						ind = mesh->indexBuffer;

						continue;
					}

					if(!strcmp(token->string, "numweights")) {
						token = token->next;
						mesh->numWeights = token->i;

						mesh->weights = (rmd5Weight_t*)malloc(sizeof(rmd5Weight_t) * mesh->numWeights);
						if(!mesh->weights) {
							COM::log << "out of memory reading " << file->Name() << std::endl;
							throw std::bad_alloc();
						}
						weight = mesh->weights;

						continue;
					}

					if(!strcmp(token->string, "vert")) {
						token = token->next;

						vert->index = token->i;
						token = token->next;

						token = token->next;
						vert->s = token->f;
						token = token->next;
						vert->t = token->f;
						token = token->next;
						token = token->next;

						vert->startWeight = token->i;
						token = token->next;

						vert->countWeight = token->i;
						token = token->next;

						vert++;

						continue;
					}

					if(!strcmp(token->string, "tri")) {
						token = token->next;

						tri->index = token->i;
						token = token->next;

						for(i = 0; i < 3; ++i) {
							tri->vertices[i] = token->i;
							ind->vertices[i] = token->i;
							token = token->next;
						}

						tri++;
						ind++;

						continue;
					}

					if(!strcmp(token->string, "weight")) {
						token = token->next;

						weight->index = token->i;
						token = token->next;

						weight->joint = token->i;
						token = token->next;

						weight->bias = token->f;
						token = token->next;

						token = token->next;
						weight->pos.x = token->f;
						token = token->next;
						weight->pos.y = token->f;
						token = token->next;
						weight->pos.z = token->f;
						token = token->next;
						token = token->next;

						weight++;

						continue;
					}

					COM_Tokenize(&tokens, lineBuffer);
				}
				mesh++;
				continue;
			}

			COM_FreeTokens(tokens);
		}
	}

	void Model::ParseAnim(GEN::Pointer<FS::File> file, rmd5Anim_t* anim) {
		char lineBuffer[LINE_BUFFER_SIZE];
		ctoken_t *tokens, *token;
		rmd5AnimJoint_t* joint;
		rmd5BaseJoint_t* baseJoint;
		rmd5Frame_t* frame;
		int i;

		while(!file->Eof()) {
			memset(lineBuffer, 0, LINE_BUFFER_SIZE);
			file->Gets(lineBuffer, LINE_BUFFER_SIZE);
			
			COM_Tokenize(&tokens, lineBuffer);
			if(NULL == tokens) continue;

			token = tokens;

			if(!strcmp(token->string, "numJoints")) {
				token = token->next;
				anim->numJoints = token->i;
				anim->joints = (rmd5AnimJoint_t*)malloc(sizeof(rmd5AnimJoint_t) * anim->numJoints);
				anim->baseFrame = (rmd5BaseJoint_t*)malloc(sizeof(rmd5BaseJoint_t) * anim->numJoints);
				if(!anim->joints || !anim->baseFrame) {
					COM::log << "out of memory reading animation file " << file->Name() << std::endl;
					throw std::bad_alloc();
				}
				COM_FreeTokens(tokens);
				tokens = NULL;
				continue;
			}

			if(!strcmp(token->string, "numFrames")) {
				token = token->next;
				anim->numFrames = token->i;
				anim->frames = (rmd5Frame_t*)malloc(sizeof(rmd5Frame_t) * anim->numFrames);
				frame = anim->frames;
				if(!anim->frames) {
					COM::log << "out of memory reading animation file " << file->Name() << std::endl;
					throw std::bad_alloc();
				}
				COM_FreeTokens(tokens);
				tokens = NULL;
				continue;
			}

			if(!strcmp(token->string, "frameRate")) {
				token = token->next;
				anim->frameRate = token->i;
				COM_FreeTokens(tokens);
				tokens = NULL;
				continue;
			}

			if(!strcmp(token->string, "numAnimatedComponents")) {
				token = token->next;
				anim->numAnimatedComponents = token->i;
				for(i = 0; i < anim->numFrames; ++i) {
					anim->frames[i].comp = (float*)malloc(sizeof(float) * anim->numAnimatedComponents);
					if(!anim->frames[i].comp) {
						COM::log << "out of memory loading animation file " << file->Name() << std::endl;
						throw std::bad_alloc();
					}
				}
				COM_FreeTokens(tokens);
				tokens = NULL;
				continue;
			}

			if(!strcmp(token->string, "hierarchy")) {
				COM_FreeTokens(tokens);
				tokens = NULL;

				joint = anim->joints;

				while(!file->Eof()) {
					memset(lineBuffer, 0, LINE_BUFFER_SIZE);
					file->Gets(lineBuffer, LINE_BUFFER_SIZE);
					if('}' == lineBuffer[0]) break;

					COM_Tokenize(&tokens, lineBuffer);
					token = tokens;

					strncpy(joint->name, token->string, MAX_JOINT_NAME);
					token = token->next;
					joint->parent = token->i;
					token = token->next;
					joint->flags = token->i;
					token = token->next;
					joint->startIndex = token->i;

					joint++;

					COM_FreeTokens(tokens);
					tokens = NULL;
				}
				continue;
			}

			if(!strcmp(token->string, "baseframe")) {
				COM_FreeTokens(tokens);
				tokens = NULL;

				baseJoint = anim->baseFrame;

				while(!file->Eof()) {
					memset(lineBuffer, 0, LINE_BUFFER_SIZE);
					file->Gets(lineBuffer, LINE_BUFFER_SIZE);
					if('}' == lineBuffer[0]) break;

					COM_Tokenize(&tokens, lineBuffer);
					token = tokens;

					token = token->next; // (
					
					baseJoint->pos.x = token->f; token = token->next;
					baseJoint->pos.y = token->f; token = token->next;
					baseJoint->pos.z = token->f; token = token->next;

					token = token->next; // )
					token = token->next; // (

					baseJoint->orient.x = token->f; token = token->next;
					baseJoint->orient.y = token->f; token = token->next;
					baseJoint->orient.z = token->f; token = token->next;

					baseJoint++;

					COM_FreeTokens(tokens);
					tokens = NULL;
				}
				continue;
			}

			if(!strcmp(token->string, "frame")) {
				COM_FreeTokens(tokens);
				tokens = NULL;

				i = 0;
				while(!file->Eof()) {
					memset(lineBuffer, 0, LINE_BUFFER_SIZE);
					file->Gets(lineBuffer, LINE_BUFFER_SIZE);
					if('}' == lineBuffer[0]) break;

					COM_Tokenize(&tokens, lineBuffer);
					token = tokens;

					while(NULL != token) {
						frame->comp[i] = token->f;
						token = token->next;
						i++;
					}
					
					COM_FreeTokens(tokens);
					tokens = NULL;
				}

				frame++;
				continue;
			}
		}
	}

	void Model::ComputeBindPose(void) {
		for(int i = 0; i < numMeshes; ++i) {
			rmd5Mesh_t* mesh = &meshes[i];

			for(int j = 0; j < mesh->numVerts; ++j) {
				rmd5Vert_t* vert = &mesh->vertices[j];
				M::Vector3 pos = M::Vector3::Zero;

				for(int k = 0; k < vert->countWeight; ++k) {
					rmd5Weight_t* weight = &mesh->weights[vert->startWeight + k];
					rmd5Joint_t* joint = &joints[weight->joint];

					M::Vector3 weightPos = M::RotateVector(weight->pos, joint->orient);
					pos += (joint->pos + weightPos) * weight->bias;
				}

				mesh->vertexBuffer[j].pos = pos;
				bbox.Add(pos);

				vert->t = 1.0f - vert->t;

				vert->numTri = 0;
				mesh->vertexBuffer[j].normal = M::Vector3::Zero;
			}

			/* compute normal, tangent */
	   
			for(int j = 0; j < mesh->numTris; ++j) {
				rmd5Tri_t* tri = &mesh->triangles[j];
				for(int k = 0; k < 3; ++k) {
					int x0 = tri->vertices[k];
					int x1 = tri->vertices[(k + 1) % 3];
					int x2 = tri->vertices[(k + 2) % 3];

					M::Vector3 v1 = mesh->vertexBuffer[x1].pos - mesh->vertexBuffer[x0].pos;
					M::Vector3 v2 = mesh->vertexBuffer[x2].pos - mesh->vertexBuffer[x0].pos;

					float d1_0 = mesh->vertices[x1].s - mesh->vertices[x0].s;
					float d1_1 = mesh->vertices[x1].t - mesh->vertices[x0].t;

					float d2_0 = mesh->vertices[x2].s - mesh->vertices[x0].s;
					float d2_1 = mesh->vertices[x2].t - mesh->vertices[x0].t;

					float det = d1_0 * d2_1 - d2_0 * d1_1;
					M::Vector3 tangent = (d2_1 * v1 - d1_1 * v2) / det;
					M::Vector3 bitangent = (-d2_0 * v1 + d1_0 * v2) / det;
					M::Vector3 normal = M::Cross(v2, v1);

					mesh->vertices[x0].numTri++;
					mesh->vertexBuffer[x0].toObjectA0 = tangent;
					mesh->vertexBuffer[x0].toObjectA1 = bitangent;
					mesh->vertexBuffer[x0].normal += normal;
				}
			}

			for(int j = 0; j < mesh->numVerts; ++j) {
				mesh->vertexBuffer[j].normal /= mesh->vertices[j].numTri;

				M::Vector3 A0 = M::Normalize(mesh->vertexBuffer[j].toObjectA0);
				M::Vector3 A1 = M::Normalize(mesh->vertexBuffer[j].toObjectA1);
				M::Vector3 A2 = M::Normalize(mesh->vertexBuffer[j].normal);

				mesh->vertexBuffer[j].toObjectA0 = A0;
				mesh->vertexBuffer[j].toObjectA1 = A1;
				mesh->vertexBuffer[j].normal = A2;

				M::Matrix3 mat = M::Mat3::FromColumns(A0, A1, A2);
				M::Matrix3 inv = M::Mat3::Inverse(mat);
				M::Mat3::ToColumns(inv, A0, A1, A2);

				mesh->vertexBuffer[j].toTangentA0 = A0;
				mesh->vertexBuffer[j].toTangentA1 = A1;
				mesh->vertexBuffer[j].toTangentA2 = A2;
			}
		}
	}

	void Model::Mesh::Draw(int flags, Program& program) const {
		GLint loc;

		glPushAttrib(GL_ENABLE_BIT);
		glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);

		glEnableClientState(GL_VERTEX_ARRAY);	
		glVertexPointer(3, GL_FLOAT, sizeof(rvert_t), &vertexBuffer->pos);

		program.Use();

		if(DF_TEXCOORD & flags) {
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(2, GL_FLOAT, sizeof(rmd5Vert_t), &vertices->s);
		}

		if(DF_TO_TANGENT & flags) {
			loc = glx.GetAttribLocation(program.GetID(), "attToTangentA0");
			glx.EnableVertexAttribArray(loc);
			glx.VertexAttribPointer(loc, 3, GL_FLOAT, false, sizeof(rvert_t), &vertexBuffer->toTangentA0);

			loc = glx.GetAttribLocation(program.GetID(), "attToTangentA1");
			glx.EnableVertexAttribArray(loc);
			glx.VertexAttribPointer(loc, 3, GL_FLOAT, false, sizeof(rvert_t), &vertexBuffer->toTangentA1);

			loc = glx.GetAttribLocation(program.GetID(), "attToTangentA2");
			glx.EnableVertexAttribArray(loc);
			glx.VertexAttribPointer(loc, 3, GL_FLOAT, false, sizeof(rvert_t), &vertexBuffer->toTangentA2);
		}

		if(DF_TO_OBJECT & flags) {
			loc = glx.GetAttribLocation(program.GetID(), "attToObjectA0");
			glx.EnableVertexAttribArray(loc);
			glx.VertexAttribPointer(loc, 3, GL_FLOAT, false, sizeof(rvert_t), &vertexBuffer->toObjectA0);

			loc = glx.GetAttribLocation(program.GetID(), "attToObjectA1");
			glx.EnableVertexAttribArray(loc);
			glx.VertexAttribPointer(loc, 3, GL_FLOAT, false, sizeof(rvert_t), &vertexBuffer->toObjectA1);
		}

		if((DF_TO_OBJECT | DF_NORMAL) & flags) {
			loc = glx.GetAttribLocation(program.GetID(), "attNormal");
			glx.EnableVertexAttribArray(loc);
			glx.VertexAttribPointer(loc, 3, GL_FLOAT, false, sizeof(rvert_t), &vertexBuffer->normal);
		}

		if(DF_TEX_DIFFUSE & flags) {
			program.SetUniform("uniTexDiffuse", 0);
			if(!texDiffuse) COM::log << "Model::Draw: attempting to bind empty texture texDiffuse" << std::endl;
			else texDiffuse->Bind(0);
		}

		if(DF_TEX_NORMAL & flags) {
			program.SetUniform("uniTexNormal", 1);
			if(!texNormal) COM::log << "Model::Draw: attempting to bind empty texture texNormal" << std::endl;
			else texNormal->Bind(1);
		}

		if(DF_TEX_ADD & flags) {
			program.SetUniform("uniTexAdd", 2);
			if(!texAdd) COM::log << "Model::Draw: attempting to bind empty texture texAdd" << std::endl;
			else texAdd->Bind(2);
		}

		if(DF_TEX_SPEC & flags) {
			program.SetUniform("uniTexSpec", 3);
			if(!texSpec) COM::log << "Model::Draw: attempting to bind empty texture texSpec" << std::endl;
			else texSpec->Bind(3);
		}

		glDrawElements(GL_TRIANGLES, numTris * 3, GL_UNSIGNED_INT, indexBuffer);

		glPopAttrib();
		glPopClientAttrib();
	}

} // namespace R