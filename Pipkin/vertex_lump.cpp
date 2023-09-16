#pragma once
#include "vertex_lump.h"
#include"memory.h"
namespace ppkin {
	VertexLump::VertexLump()
	{
		offset = 0;
	}
	VertexLump::~VertexLump() {
		vkDestroyBuffer(logicalDevice, vertexBuffer.buffer, nullptr);
		vkFreeMemory(logicalDevice, vertexBuffer.bufferMemory, nullptr);
	}
	void VertexLump::consume(std::vector<float> vertexData){
		for (float attribute : vertexData) {
			lump.push_back(attribute);
		}
		int vertexCount = static_cast<int>(vertexData.size()/5);
		offsets.push_back(offset);
		size.push_back(vertexCount);
		offset += vertexCount;
	}
	void VertexLump::finalize(logicDevice* devices)
	{
		this->logicalDevice = devices->getLogicalDevice();

		BufferInput inputChunk;
		inputChunk.devices = devices;
		inputChunk.size = sizeof(float) * lump.size();
		inputChunk.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

		vertexBuffer = createBuffer(inputChunk);
		void* memoryLocation;
		if (vkMapMemory(logicalDevice, vertexBuffer.bufferMemory, 0, inputChunk.size, 0, &memoryLocation) != VK_SUCCESS) {
			throw std::exception("failed to map memory!");
		}
		else {
			std::cout << ("successfully mapped memory") << std::endl;
		}
		memcpy(memoryLocation, lump.data(), inputChunk.size);
		vkUnmapMemory(logicalDevice, vertexBuffer.bufferMemory);
	}
}
