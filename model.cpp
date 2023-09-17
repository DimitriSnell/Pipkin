#pragma once
#include "model.h"
namespace ppkin {
	model::model(logicDevice* devices, const std::vector<Vertex>& verticies)
	{
		this->devices = devices;
		BufferInput inputChunk;
		inputChunk.devices = devices;
		vertexCount = static_cast<uint32_t>(verticies.size());
		inputChunk.size = sizeof(verticies[0]) * vertexCount;
		inputChunk.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		buffer = createBuffer(inputChunk);
		void* memoryLocation;
		if (vkMapMemory(devices->getLogicalDevice(), buffer.bufferMemory, 0, inputChunk.size, 0, &memoryLocation) != VK_SUCCESS) {
			throw std::exception("failed to map memory!");
		}
		else {
			std::cout << ("successfully mapped memory") << std::endl;
		}
		memcpy(memoryLocation, verticies.data(), static_cast<size_t>(inputChunk.size));
		vkUnmapMemory(devices->getLogicalDevice(), buffer.bufferMemory);
	}
	void model::destroyModel(VkDevice device)
	{
		vkDestroyBuffer(device, buffer.buffer, nullptr);
		vkFreeMemory(device, buffer.bufferMemory, nullptr);
	}
	void model::draw(VkCommandBuffer commandBuffer)
	{
		vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
	}
	void model::bind(VkCommandBuffer commandBuffer)
	{
		//VkBuffer vertexBuffers[] = { meshes->vertexBuffer.buffer };
		VkBuffer vertexBuffers[] = { buffer.buffer };
		VkDeviceSize offset[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offset);

	}
}
