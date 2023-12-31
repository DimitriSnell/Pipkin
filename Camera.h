#pragma once
#include"config.h"

namespace ppkin {
	class Camera {
	public:

		void setOrthoGraphicProjection(float left, float right, float top, float bottom, float near, float far);
		void setPerspectiveProjection(float fovy, float aspect, float near, float far);


		const glm::mat4& getProjection() {
			return projectionMatrix;
		}

		const glm::mat4& getView() {
			return viewMatrix;
		}

		void setViewDirection(glm::vec3 position, glm::vec3 direction, glm::vec3 up = {0.f,-1.f,0.f}) {
			viewMatrix = glm::lookAt(position, direction, up);
		}

		void seTargetDirection(glm::vec3 position, glm::vec3 target, glm::vec3 up) {
			setViewDirection(position, target - position, up);
		}

		void setViewYXZ(glm::vec3 position, glm::vec3 rotation) {
			const float c3 = glm::cos(rotation.z);
			const float s3 = glm::sin(rotation.z);
			const float c2 = glm::cos(rotation.x);
			const float s2 = glm::sin(rotation.x);
			const float c1 = glm::cos(rotation.y);
			const float s1 = glm::sin(rotation.y);
			const glm::vec3 u{ (c1 * c3 + s1 * s2 * s3), (c2 * s3), (c1 * s2 * s3 - c3 * s1) };
			const glm::vec3 v{ (c3 * s1 * s2 - c1 * s3), (c2 * c3), (c1 * c3 * s2 + s1 * s3) };
			const glm::vec3 w{ (c2 * s1), (-s2), (c1 * c2) };
			viewMatrix = glm::mat4{ 1.f };
			viewMatrix[0][0] = u.x;
			viewMatrix[1][0] = u.y;
			viewMatrix[2][0] = u.z;
			viewMatrix[0][1] = v.x;
			viewMatrix[1][1] = v.y;
			viewMatrix[2][1] = v.z;
			viewMatrix[0][2] = w.x;
			viewMatrix[1][2] = w.y;
			viewMatrix[2][2] = w.z;
			viewMatrix[3][0] = -glm::dot(u, position);
			viewMatrix[3][1] = -glm::dot(v, position);
			viewMatrix[3][2] = -glm::dot(w, position);
		
		}
		
	private:
		glm::mat4 projectionMatrix{ 1.f };
		glm::mat4 viewMatrix{ 1.f };
	};


}