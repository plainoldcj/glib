/*
solution:	glib
project:	prenderer
			spheres
file:		camera.cpp
author:		cj
*/

#include "../system/glx.h"
#include "camera.h"

namespace R {

	M::Vector3 Camera::VectorFromMousePos(int mouseX, int mouseY) {
		float x = mouseX - _halfWidth;
		float y = -mouseY + _halfHeight;
		const float r2 = _radius * _radius;
		const float len2 = x * x + y * y;
		M::Vector3 ret;
		if(len2 > r2) {
			ret = M::Normalize(M::Vector3(x, y, 0.0f));
		} else {
			const float z = sqrt(r2 - len2);
			ret = M::Normalize(M::Vector3(x, y, z));
		}
		return ret;
	}

	Camera::Camera(int width, int height)
		: _dragging(false),
		  _rotation(M::Mat3::Identity()),
		  _dragRot(M::Mat3::Identity())
	{
		SetScreenSize(width, height);
	}

	void Camera::SetScreenSize(int width, int height) {
		_halfWidth = width / 2.0f;
		_halfHeight = height / 2.0f;
		_radius = M::Min(_halfWidth, _halfHeight);
	}

	void Camera::StartDragging(int mouseX, int mouseY) {
		if(!_dragging) {
			_v0 = VectorFromMousePos(mouseX, mouseY);
			_dragging = true;
		}
	}

	void Camera::Drag(int mouseX, int mouseY) {
		if(_dragging) {
			const M::Vector3 v1 = VectorFromMousePos(mouseX, mouseY);
			if(M::LinearlyDependent(_v0, v1)) return;
			const M::Vector3 axis = M::Normalize(M::Cross(_v0, v1));
			float angle = acosf(M::Dot(_v0, v1));
			_dragRot = _dragRot * M::Mat3::RotateAxis(axis, M::Rad2Deg(angle));
			_v0 = v1;
		}
	}

	void Camera::StopDragging(void) {
		_dragging = false;
		_rotation = _dragRot * _rotation;
		_dragRot = M::Mat3::Identity();
	}

	M::Matrix4 Camera::GetMatrix(void) const {
		if(_dragging) {
			return M::Mat4::FromRigidTransform(_dragRot * _rotation, M::Vector3::Zero);
		} else {
			return M::Mat4::FromRigidTransform(_rotation, M::Vector3::Zero);
		}
	}

} // namespace R