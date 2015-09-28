/*
solution:	glib
project:	renderer
file:		model.cpp
author:		cj
*/

#include "renderer.h"

#define LINE_BUFFER_SIZE 4096

int R_LoadModelMD5(rmd5Model_t* model, const char* filename) {
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
	float t;
	int x0, x1, x2;
	float det;
	M::Vector3 tangent;
	M::Vector3 bitangent;
	M::Vector3 normal;
	M::Vector3 v, v1, v2;
	float d1_0, d1_1;
	float d2_0, d2_1;
	M::Vector3 a0, a1, a2;
	int i, j, k;
	
	COM_assert(NULL != model);

	FILE* file = fopen(filename, "r");
	if(!file) {
		COM::log << "unable to open file " << filename << std::endl;
		return 1;
	}

	model->numJoints = model->numMeshes = -1;
	
	while(!feof(file)) {
		memset(lineBuffer, 0, LINE_BUFFER_SIZE);
		fgets(lineBuffer, LINE_BUFFER_SIZE, file);
		
		COM_Tokenize(&tokens, lineBuffer);
		token = tokens;

		if(NULL == token) continue;

		if(!strcmp(token->string, "numJoints")) {
			token = token->next;
			model->numJoints = token->i;

			model->joints = (rmd5Joint_t*)malloc(sizeof(rmd5Joint_t) * model->numJoints);
			if(!model->joints) {
				COM::log << "out of memory reading " << filename << std::endl;
				return 0;
			}
			joint = model->joints;

			continue;
		}

		if(!strcmp(token->string, "numMeshes")) {
			token = token->next;
			model->numMeshes = token->i;

			model->meshes = (rmd5Mesh_t*)malloc(sizeof(rmd5Mesh_t) * model->numMeshes);
			if(!model->meshes) {
				COM::log << "out of memory reading " << filename << std::endl;
				return 0;
			}
			mesh = model->meshes;

			continue;
		}

		if(!strcmp(token->string, "joints")) {
			COM_FreeTokens(tokens);
			tokens = NULL;

			while(!feof(file)) {
				memset(lineBuffer, 0, LINE_BUFFER_SIZE);
				fgets(lineBuffer, LINE_BUFFER_SIZE, file);
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

				t = 1.0f
					- (joint->orient.x * joint->orient.x)
					- (joint->orient.y * joint->orient.y)
					- (joint->orient.z * joint->orient.z);
				if(t < 0.0f) joint->orient.w = 0.0f;
				else joint->orient.w = -sqrt(t);

				joint++;

				COM_FreeTokens(tokens);
			}
			continue;
		} // reading joints

		if(!strcmp(token->string, "mesh")) {
			COM_FreeTokens(tokens);
			tokens = NULL;

			mesh->texDiffuse = NULL;
			mesh->texNormal = NULL;
			mesh->texAdd = NULL;
			mesh->texSpec = NULL;

			while(!feof(file)) {
				memset(lineBuffer, 0, LINE_BUFFER_SIZE);
				fgets(lineBuffer, LINE_BUFFER_SIZE, file);
				if('}' == lineBuffer[0]) break;

				COM_Tokenize(&tokens, lineBuffer);
				token = tokens;

				if(!strcmp(token->string, "numverts")) {
					token = token->next;
					mesh->numVerts = token->i;

					mesh->vertices = (rmd5Vert_t*)malloc(sizeof(rmd5Vert_t) * mesh->numVerts);
					mesh->vertexBuffer = (rvert_t*)malloc(sizeof(rvert_t) * mesh->numVerts);
					
					if(!mesh->vertices || !mesh->vertexBuffer) {
						COM::log << "out of memory reading " << filename << std::endl;
						return 0;
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
						COM::log << "out of memory reading " << filename << std::endl;
						return 0;
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
						COM::log << "out of memory reading " << filename << std::endl;
						return 0;
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

	fclose(file);

	for(i = 0; i < model->numMeshes; ++i) {
		mesh = &model->meshes[i];

		/* compute bind pose */
		for(j = 0; j < mesh->numVerts; ++j) {
			vert = &mesh->vertices[j];
			pos = M::Vector3::Zero;

			for(k = 0; k < vert->countWeight; ++k) {
				weight = &mesh->weights[vert->startWeight + k];
				joint = &model->joints[weight->joint];

				weightPos = M::RotateVector(weight->pos, joint->orient);
				pos += (joint->pos + weightPos) * weight->bias;
			}

			mesh->vertexBuffer[j].pos = pos;
			model->bbox.Add(pos);

			vert->t = 1.0f - vert->t;

			vert->numTri = 0;
			mesh->vertexBuffer[j].normal = M::Vector3::Zero;
			mesh->vertexBuffer[j].tangent = M::Vector3::Zero;
			mesh->vertexBuffer[j].bitangent = M::Vector3::Zero;
		}

		/* compute normal, tangent */
	   
		for(j = 0; j < mesh->numTris; ++j) {
			tri = &mesh->triangles[j];
			for(k = 0; k < 3; ++k) {
				x0 = tri->vertices[k];
				x1 = tri->vertices[(k + 1) % 3];
				x2 = tri->vertices[(k + 2) % 3];

				v1 = mesh->vertexBuffer[x1].pos - mesh->vertexBuffer[x0].pos;
				v2 = mesh->vertexBuffer[x2].pos - mesh->vertexBuffer[x0].pos;

				d1_0 = mesh->vertices[x1].s - mesh->vertices[x0].s;
				d1_1 = mesh->vertices[x1].t - mesh->vertices[x0].t;

				d2_0 = mesh->vertices[x2].s - mesh->vertices[x0].s;
				d2_1 = mesh->vertices[x2].t - mesh->vertices[x0].t;

				det = d1_0 * d2_1 - d2_0 * d1_1;
				tangent = (d2_1 * v1 - d1_1 * v2) / det;
				bitangent = (-d2_0 * v1 + d1_0 * v2) / det;
				normal = M::Cross(v2, v1);

				mesh->vertices[x0].numTri++;
				mesh->vertexBuffer[x0].tangent = tangent;
				mesh->vertexBuffer[x0].bitangent = bitangent;
				mesh->vertexBuffer[x0].normal += normal;
			}
		}

		for(j = 0; j < mesh->numVerts; ++j) {
			mesh->vertexBuffer[j].normal /= mesh->vertices[j].numTri;

			tangent = M::Normalize(mesh->vertexBuffer[j].tangent);
			bitangent = M::Normalize(mesh->vertexBuffer[j].bitangent);
			normal = M::Normalize(mesh->vertexBuffer[j].normal);

			mesh->vertexBuffer[j].invTangent = tangent;
			mesh->vertexBuffer[j].invBitangent = bitangent;
			mesh->vertexBuffer[j].invNormal = normal;

			M::Matrix3 mat = M::Mat3::FromColumns(tangent, bitangent, normal);
			M::Matrix3 inv = M::Mat3::Inverse(mat);
			M::Mat3::ToColumns(inv, tangent, bitangent, normal);

			mesh->vertexBuffer[j].tangent = tangent;
			mesh->vertexBuffer[j].bitangent = bitangent;
			mesh->vertexBuffer[j].normal = normal;
		}
	}

	return 0;
}

void R_DrawNormalsMD5(rmd5Model_t* model, int index) {
	rmd5Mesh_t* mesh;
	M::Vector3 v0, v1;
	int i;

	COM_assert(NULL != model);
	COM_assert(index < model->numMeshes);

	mesh = &model->meshes[index];

	glColor3f(1.0f, 0.0f, 1.0f);
	glBegin(GL_LINES);
	for(i = 0; i < mesh->numVerts; ++i) {
		v0 = mesh->vertexBuffer[i].pos;
		v1 = v0 + mesh->vertexBuffer[i].invNormal;
		glVertex3f(v0.x, v0.y, v0.z);
		glVertex3f(v1.x, v1.y, v1.z);
	}
	glEnd();
	glColor3f(1.0f, 1.0f, 1.0f);
}

void R_DrawTangentSpaceMD5(rmd5Model_t* model, int index) {
	rmd5Mesh_t* mesh;
	M::Vector3 v0, v1;
	int i;

	COM_assert(NULL != model);
	COM_assert(index < model->numMeshes);

	mesh = &model->meshes[index];

	glColor3f(1.0f, 0.0f, 1.0f);
	glBegin(GL_LINES);
	for(i = 0; i < mesh->numVerts; ++i) {
		v0 = mesh->vertexBuffer[i].pos;

		v1 = v0 + 1.0f * M::Normalize(mesh->vertexBuffer[i].tangent);
		glVertex3f(v0.x, v0.y, v0.z);
		glVertex3f(v1.x, v1.y, v1.z);

		v1 = v0 + 1.0f * M::Normalize(mesh->vertexBuffer[i].bitangent);
		glVertex3f(v0.x, v0.y, v0.z);
		glVertex3f(v1.x, v1.y, v1.z);
	}
	glEnd();
	glColor3f(1.0f, 1.0f, 1.0f);
}

void R_DrawMeshMD5(rmd5Model_t* model, GLuint program, int index) {
	rmd5Mesh_t* mesh;
	GLint tangentLoc, bitangentLoc;
	GLint invTangentLoc, invBitangentLoc, invNormalLoc;
	
	COM_assert(NULL != model);
	COM_assert(index < model->numMeshes);

	mesh = &model->meshes[index];

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	glVertexPointer(3, GL_FLOAT, sizeof(rvert_t), &mesh->vertexBuffer->pos);
	glNormalPointer(GL_FLOAT, sizeof(rvert_t), &mesh->vertexBuffer->normal);
	glTexCoordPointer(2, GL_FLOAT, sizeof(rmd5Vert_t), &mesh->vertices->s);

	if(program) {
		tangentLoc = glx.GetAttribLocation(program, "tangent");
		glx.EnableVertexAttribArray(tangentLoc);
		glx.VertexAttribPointer(tangentLoc, 3, GL_FLOAT, false, sizeof(rvert_t), &mesh->vertexBuffer->tangent);
	
		bitangentLoc = glx.GetAttribLocation(program, "bitangent");
		glx.EnableVertexAttribArray(bitangentLoc);
		glx.VertexAttribPointer(bitangentLoc, 3, GL_FLOAT, false, sizeof(rvert_t), &mesh->vertexBuffer->bitangent);

		invTangentLoc = glx.GetAttribLocation(program, "invTangent");
		glx.EnableVertexAttribArray(invTangentLoc);
		glx.VertexAttribPointer(invTangentLoc, 3, GL_FLOAT, false, sizeof(rvert_t), &mesh->vertexBuffer->invTangent);

		invBitangentLoc = glx.GetAttribLocation(program, "invBitangent");
		glx.EnableVertexAttribArray(invBitangentLoc);
		glx.VertexAttribPointer(invBitangentLoc, 3, GL_FLOAT, false, sizeof(rvert_t), &mesh->vertexBuffer->invBitangent);

		invNormalLoc = glx.GetAttribLocation(program, "invNormal");
		glx.EnableVertexAttribArray(invNormalLoc);
		glx.VertexAttribPointer(invNormalLoc, 3, GL_FLOAT, false, sizeof(rvert_t), &mesh->vertexBuffer->invNormal);
	}

	if(mesh->texDiffuse) {
		glx.ActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->texDiffuse->id);
	}

	if(mesh->texNormal) {
		glx.ActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, mesh->texNormal->id);
	}

	if(mesh->texAdd) {
		glx.ActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, mesh->texAdd->id);
	}

	if(mesh->texSpec) {
		glx.ActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, mesh->texSpec->id);
	}

	glDrawElements(GL_TRIANGLES, mesh->numTris * 3, GL_UNSIGNED_INT, mesh->indexBuffer);

	glx.ActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glx.ActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);
	glx.ActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, 0);
	glx.ActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, 0);
	glx.ActiveTexture(GL_TEXTURE0);

	if(program) {
		glx.DisableVertexAttribArray(tangentLoc);
		glx.DisableVertexAttribArray(bitangentLoc);

		glx.DisableVertexAttribArray(invTangentLoc);
		glx.DisableVertexAttribArray(invBitangentLoc);
	}

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}