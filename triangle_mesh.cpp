#pragma once
#include "triangle_mesh.h"

ppkin::TriangleMesh::TriangleMesh(logicDevice* devices)
{
	this->logicalDevice = devices->getLogicalDevice();

	std::vector<float> verticies = { {
				0.0f, -0.5f, 1.0f, 1.0f, 1.0f,
	0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
	-0.5f, 0.5f, 0.0f, 0.0f, 1.0f

} };
	BufferInput inputChunk;
	inputChunk.devices = devices;
	inputChunk.size = sizeof(float) * verticies.size();
	inputChunk.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

	vertexBuffer = createBuffer(inputChunk);
	void* memoryLocation;
	if (vkMapMemory(logicalDevice, vertexBuffer.bufferMemory, 0, inputChunk.size, 0, &memoryLocation) != VK_SUCCESS) {
		throw std::exception("failed to map memory!");
	}
	else {
		std::cout << ("successfully mapped memory") << std::endl;
	}
	memcpy(memoryLocation, verticies.data(), inputChunk.size);
	vkUnmapMemory(logicalDevice, vertexBuffer.bufferMemory);
}


ppkin::TriangleMesh::~TriangleMesh() {
	vkDestroyBuffer(logicalDevice,vertexBuffer.buffer,nullptr);
	vkFreeMemory(logicalDevice, vertexBuffer.bufferMemory, nullptr);
}