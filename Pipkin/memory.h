#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "logical_device.hpp"
namespace ppkin {
	struct BufferInput {
		size_t size;
		VkBufferUsageFlags usage;
		logicDevice* devices;
	};

	struct Buffer {
		VkBuffer buffer;
		VkDeviceMemory bufferMemory;
	};

	Buffer createBuffer(BufferInput input);

	uint32_t findMemoryTypeIndex(VkPhysicalDevice Pdevice, uint32_t supportedMemoryIndices, VkMemoryPropertyFlags requestedProperties);

	void allocatebufferMemory(Buffer& buffer, BufferInput& input);

}