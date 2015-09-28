/*
bbox.h
*/

#include "math3d.h"

namespace M {

	struct BBox {
		M::Vector3 llb; // lower left back (<)
		M::Vector3 urf; // upper right front (>)

		BBox(void) : llb(M::Vector3::Zero), urf(M::Vector3::Zero) { }

		void Add(const M::Vector3& p) {
			if(p.x < llb.x) llb.x = p.x;
			if(p.y < llb.y) llb.y = p.y;
			if(p.z < llb.z) llb.z = p.z;

			if(p.x > urf.x) urf.x = p.x;
			if(p.y > urf.y) urf.y = p.y;
			if(p.z > urf.z) urf.z = p.z;
		}

		M::Vector3 Center(void) const {
			return llb + (urf - llb) / 2;
		}
	};

} // namespace M