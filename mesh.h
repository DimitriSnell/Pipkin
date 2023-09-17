#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include<array>
#include"model.h"
#include<cstddef>
namespace ppkin {
	VkVertexInputBindingDescription getPosColorBindingDescription() {
		VkVertexInputBindingDescription bindingdescription;
		bindingdescription.binding = 0;
		bindingdescription.stride = sizeof(model::Vertex);
		bindingdescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		return bindingdescription;
	}

	std::array<VkVertexInputAttributeDescription, 2> getPosColorAttributeDescriptions() {
		std::array<VkVertexInputAttributeDescription, 2> attributes;

		//position
		attributes[0].binding = 0;
		attributes[0].location = 0;
		attributes[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributes[0].offset = offsetof(model::Vertex, position);

		//color
		attributes[1].binding = 0;
		attributes[1].location = 1;
		attributes[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributes[1].offset = offsetof(model::Vertex, color);

		return attributes;
	}
};