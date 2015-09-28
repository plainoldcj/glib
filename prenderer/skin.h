/*
solution:	glib
project:	prenderer
file:		skin.h
author:		cj
*/

#pragma once

#include "texture.h"
#include "color.h"

namespace R {

	class Program;

	enum BindFlags {
		BF_TEX_DIFFUSE = (1 << 0),
		BF_TEX_NORMAL = (1 << 1),
		BF_TEX_ADD = (1 << 2),
		BF_TEX_SPEC = (1 << 3)
	};

	class Skin {
	public:
		enum TexLayer {
			TL_DIFFUSE = 0,
			TL_NORMAL,
			TL_ADDITIVE,
			TL_SPECULAR
		};
		enum { NUM_LAYERS = TL_SPECULAR + 1 };
	private:
		GEN::Pointer<Texture> _textures[NUM_LAYERS];
		Color _color;
	public:
		Skin(void) : _color(1.0f, 1.0f, 1.0f) { }

		void SetTexture(TexLayer layer, const GEN::Pointer<Texture>& texture) {
			_textures[layer] = texture;
		}

		void SetColor(const Color& color) { _color = color; }
		const Color& GetColor(void) const { return _color; }

		void Bind(int flags, Program& program) const;
		void BindFFP(int flags) const;
	};

} // namespace R