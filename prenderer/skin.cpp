/*
solution:	glib
project:	prenderer
file:		skin.cpp
author:		cj
*/

#include "program.h"
#include "skin.h"

namespace R {

	void Skin::Bind(int flags, Program& program) const {
		static const char* names[] = {
			"uniTexDiffuse", "uniTexNormal", "uniTexAdd", "uniTexSpec"
		};

		for(int i = 0; i < NUM_LAYERS; ++i) {
			if((1 << i) & flags) {
				if(!_textures[i]) COM::log << "Skin::Bind: attempting to bind non-existent texture on layer " << names[i] << std::endl;
				else {
					program.SetUniform(names[i], i);
					_textures[i]->Bind(i);
				}
			} else {
				glx.ActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, 0);
			}
		}
	}

	void Skin::BindFFP(int flags) const {
		static const char* names[] = {
			"uniTexDiffuse", "uniTexNormal", "uniTexAdd", "uniTexSpec"
		};

		for(int i = 0; i < NUM_LAYERS; ++i) {
			if((1 << i) & flags) {
				if(!_textures[i]) COM::log << "Skin::BindFFP: attempting to bind non-existent texture on layer " << names[i] << std::endl;
				else {
					_textures[i]->Bind(i);
				}
			} else {
				glx.ActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, 0);
			}
		}
	}

} // namespace R