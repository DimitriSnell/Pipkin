#pragma once
#include "memory.h"
namespace ppkin {
	Buffer createBuffer(BufferInput input)
	{
		VkBufferCreateInfo bufferInfo;
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.flags = VkBufferCreateFlags();
		bufferInfo.size = input.size;
		bufferInfo.usage = input.usage;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		bufferInfo.pNext = nullptr;

		Buffer buffer;
		if (vkCreateBuffer(input.devices->getLogicalDevice(), &bufferInfo, nullptr, &buffer.buffer) != VK_SUCCESS) {
			throw std::exception("Failed to create buffer!");
		}
		allocatebufferMemory(buffer, input);
		return buffer;
	}
	uint32_t findMemoryTypeIndex(VkPhysicalDevice Pdevice, uint32_t supportedMemoryIndices, VkMemoryPropertyFlags requestedProperties)
	{
		VkPhysicalDeviceMemoryProperties memoryProperties;
		vkGetPhysicalDeviceMemoryProperties(Pdevice, &memoryProperties);
		std::cout << "looking for index" << std::endl;
		for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++) {
			bool supported{ static_cast<bool>(supportedMemoryIndices & (1 << i)) };

			bool sufficient{ (memoryProperties.memoryTypes[i].propertyFlags & requestedProperties) == requestedProperties };

			if (supported && sufficient) {
				std::cout << "found index & " << i << std::endl;
				return i;
			}
		};
	}
	void allocatebufferMemory(Buffer& buffer, BufferInput& input)
	{
		VkMemoryRequirements memoryRequirements;
		vkGetBufferMemoryRequirements(input.devices->getLogicalDevice(), buffer.buffer, &memoryRequirements);

		VkMemoryAllocateInfo allocInfo;
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memoryRequirements.size;
		allocInfo.memoryTypeIndex = findMemoryTypeIndex(input.devices->getPhysicalDevice(), memoryRequirements.memoryTypeBits,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		allocInfo.pNext = nullptr;

		if (vkAllocateMemory(input.devices->getLogicalDevice(), &allocInfo, nullptr, &buffer.bufferMemory) != VK_SUCCESS) {
			throw std::exception("Failed to allocate memory!");
		}
		if (vkBindBufferMemory(input.devices->getLogicalDevice(), buffer.buffer, buffer.bufferMemory, 0) != VK_SUCCESS) {
			throw std::exception("Failed to bind buffer!");
		}
	}



	
}