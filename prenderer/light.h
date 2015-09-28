/*
solution:	glib
project:	prenderer
file:		light.h
author:		cj
*/

#pragma once

#include "../common/math3d.h"
#include "color.h"

namespace R {

	class Program;

	class Light {
	private:
		M::Vector3 _position;

		// attenuation factors
		float _constantAtt;
		float _linearAtt;
		float _quadricAtt;

		Color _diffuseColor;
	public:
		enum BindFlags {
			BF_POSITION = (1 << 0),
			BF_COLOR = (1 << 1),
			BF_ATTENUATION = (1 << 2)
		};

		Light(void);

		// set uniforms
		void Bind(int flags, const M::Matrix4& world, Program& program) const;

		void SetPosition(const M::Vector3& position) { _position = position; }
		const M::Vector3& GetPosition(void) const { return _position; }

		void SetConstantAttenuation(float att) { _constantAtt = att; }
		void SetLinearAttenuation(float att) { _linearAtt = att; }
		void SetQuadricAttenuation(float att) { _quadricAtt = att; }

		float GetConstantAttenuation(float att) const { return _constantAtt; }
		float GetLinearAttenuation(float att) const { return _linearAtt; }
		float GetQuadricAttenuation(float att) const { return _quadricAtt; }

		void SetDiffuseColor(const Color& color) { _diffuseColor = color; }
		const Color& GetDiffuseColor(void) const { return _diffuseColor; }
	};

} // namespace R