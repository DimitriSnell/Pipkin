#pragma once
#include"config.h"
#include"memory.h"
//#include"mesh.h"
#include"logical_device.hpp"
namespace ppkin {
	class model {
	public:
		struct Vertex {
			glm::vec3 position{};
			glm::vec3 color{};

			
		};

		model(logicDevice* devices, const std::vector<Vertex> &verticies);
		
		void destroyModel(VkDevice device);
		void draw(VkCommandBuffer commandBuffer);
		void bind(VkCommandBuffer commandBuffer);
		Buffer buffer;
	private:
		
		logicDevice* devices;
		uint32_t vertexCount;

	};

}