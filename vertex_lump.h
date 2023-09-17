#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include<vector>
#include<unordered_map>
#include"logical_device.hpp"
#include"memory.h"
namespace ppkin {
	class VertexLump {
	public:
		VertexLump();
		~VertexLump();
		void consume(std::vector<float>vertexData);
		void finalize(logicDevice* devices);
		Buffer vertexBuffer;
		int offset;
		std::vector<int> offsets;
		std::vector<int> size;
		//std::unordered_map<
	private:
		std::vector<float> lump;
		VkDevice logicalDevice;
	};
}