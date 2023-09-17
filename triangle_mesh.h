#pragma once
#include"logical_device.hpp"
#include"memory.h"
namespace ppkin {
	class TriangleMesh {

	public:

		TriangleMesh(logicDevice* devices);
		~TriangleMesh();
		Buffer vertexBuffer;
	private:
		VkDevice logicalDevice;
	};
}