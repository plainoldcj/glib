/*
solution:	glib
project:	prenderer
file:		light.cpp
author:		cj
*/

#include "light.h"
#include "program.h"

namespace R {

	Light::Light(void)
		: _position(M::Vector3::Zero),
		  _constantAtt(1.0f),
		  _linearAtt(0.0f),
		  _quadricAtt(0.0f),
		  _diffuseColor(1.0f, 1.0f, 1.0f)
	{ }

	void Light::Bind(int flags, const M::Matrix4& world, Program& program) const {
		if(BF_POSITION & flags) {
			M::Vector3 pos = M::Mat4::Transform(world, _position);
			program.SetUniform("uniLightPos", pos);
		}

		if(BF_COLOR & flags) {
			program.SetUniform("uniLightDiffuseCol", _diffuseColor);
		}

		if(BF_ATTENUATION & flags) {
			program.SetUniform("uniLightConstantAtt", _constantAtt);
			program.SetUniform("uniLightLinearAtt", _linearAtt);
			program.SetUniform("uniLightQuadricAtt", _quadricAtt);
		}
	}

} // namespace R