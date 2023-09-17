#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include<vector>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
namespace ppkin {
	struct ObjectData {
		glm::mat4 model{ 1.f };
		alignas(16)glm::vec3 color;
	};

}
