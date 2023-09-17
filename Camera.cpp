#pragma once
#include "Camera.h"
#include<cassert>
#include<limits>

namespace ppkin {
	void Camera::setOrthoGraphicProjection(float left, float right, float top, float bottom, float near, float far)
	{
		projectionMatrix = glm::mat4{ 1.0f };
		projectionMatrix[0][0] = 2.f / (right - left);
		projectionMatrix[1][1] = 2.f / (bottom - top);
		projectionMatrix[2][2] = 1.f / (far - near);
		projectionMatrix[3][0] = -(right + left) / (right - left);
		projectionMatrix[3][1] = -(bottom + top) / (bottom - top);
		projectionMatrix[3][2] = -near / (far - near);
	}
	void Camera::setPerspectiveProjection(float fovy, float aspect, float near, float far)
	{
		projectionMatrix = glm::perspective(glm::radians(fovy),(aspect), 0.1f, 100.0f);
	}
}
