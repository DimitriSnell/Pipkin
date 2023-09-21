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

	std::array<VkVertexInputAttributeDescription, 5> getPosColorAttributeDescriptions() {
		std::array<VkVertexInputAttributeDescription, 5> attributes;

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

		//color
		attributes[2].binding = 0;
		attributes[2].location = 2;
		attributes[2].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributes[2].offset = offsetof(model::Vertex, normal);

		//color
		attributes[3].binding = 0;
		attributes[3].location = 3;
		attributes[3].format = VK_FORMAT_R32G32_SFLOAT;
		attributes[3].offset = offsetof(model::Vertex, uv);

		//color
		attributes[4].binding = 0;
		attributes[4].location = 4;
		attributes[4].format = VK_FORMAT_R32_SINT;
		attributes[4].offset = offsetof(model::Vertex, textureIndex);
		return attributes;
	}
};