/*
solution:	glib
project:	prenderer
			spheres
file:		camera.h
author:		cj
*/

/*
note:
this implementation uses matrices instead of quaternions since
by time of this writing i was too lazy to extend the M::Quaternion 
class. additionally i do not take care of numerical drift, because 
it simply wasnt an issue until now.
*/

#pragma once

#include "../math/vector3.h"
#include "../math/matrix3.h"
#include "../math/matrix4.h"

namespace R {

	class Camera {
	private:
		float _halfWidth;
		float _halfHeight;
		float _radius;

		bool _dragging;
		M::Vector3 _v0;
		M::Matrix3 _dragRot;
		M::Matrix3 _rotation;

		M::Vector3 VectorFromMousePos(int mouseX, int mouseY);
	public:
		Camera(int width, int height);

		void SetScreenSize(int width, int height);

		void StartDragging(int mouseX, int mouseY);
		void Drag(int mouseX, int mouseY);
		void StopDragging(void);

		M::Matrix4 GetMatrix(void) const;
	};

} // namespace R