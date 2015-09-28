/*
solution:	glib
project:	renderutils
file:		vat_model.cpp
author:		cj
*/

#include "../common/common.h"
#include "vat_model.h"

#define LINE_BUFFER_SIZE 2048
#define NUM_VERT_COMP 4
#define NUM_TRI_COMP 4

namespace R {
namespace UTIL {

	VATModel::VATModel(GEN::Pointer<FS::File> file) {
		GEN::Pointer<VATMesh> mesh(new VATMesh());

		char lineBuffer[LINE_BUFFER_SIZE];
		ctoken_t *tokens = NULL, *token;

		while(!file->Eof()) {
			memset(lineBuffer, 0, LINE_BUFFER_SIZE);
			file->Gets(lineBuffer, LINE_BUFFER_SIZE);

			if(tokens) {
				COM_FreeTokens(tokens);
				tokens = NULL;
			}

			int numTokens = COM_Tokenize(&tokens, lineBuffer);
			if(!numTokens) continue;

			token = tokens;

			if(2 <= strlen(token->string)
				&& token->string[0] == '/'
				&& token->string[1] == '/')
			{
				continue;
			}

			if(!strcmp("vert", token->string) && NUM_VERT_COMP == numTokens) {
				token = token->next;

				Vertex vertex;
				
				for(int i = 0; i < 3; ++i) {
					vertex.pos.vec[i] = token->f;
					token = token->next;
				}
				/*vertex.texCoord.s = token->f;
				token = token->next;
				vertex.texCoord.t = token->f;
				token = token->next;*/

				mesh->vertices.push_back(vertex);
				continue;
			}

			if(!strcmp("tri", token->string) && NUM_TRI_COMP == numTokens) {
				token = token->next;

				Triangle triangle;
				for(int i = 0; i < 3; ++i) {
					triangle.vertices[i] = token->i;
					token = token->next;
				}
				mesh->triangles.push_back(triangle);
				continue;
			}

			if(!strcmp("push", token->string)) {
				_meshes.push_back(mesh);
				mesh = GEN::Pointer<VATMesh>(new VATMesh());
				continue;
			}
		}
	}

} // namespace UTIL
} // namespace R