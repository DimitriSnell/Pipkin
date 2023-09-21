#pragma once
#include"config.h"
#include"memory.h"

namespace ppkin {
	struct ImageInputChunk {
		VkDevice logicalDevice;
		VkPhysicalDevice physicalDevice;
		int width;
		int height;
		VkImageTiling tiling;
		VkImageUsageFlags usage;
		VkMemoryPropertyFlags memoryProperties;
		VkFormat format;

	};

	VkImageView makeImageViewTexture(VkDevice logicalDevice, VkImage image, VkFormat format, VkImageAspectFlags aspect);

	VkImageView makeImageViewDepth(VkDevice logicalDevice, VkImage image, VkFormat format, VkImageAspectFlags aspect);

	VkImage make_image(ImageInputChunk input);


	VkDeviceMemory makeMemory(ImageInputChunk input, VkImage image);

	VkCommandBuffer beginSingleTimeCommands(logicDevice* devices);

	void endSingleTimeCommands(VkCommandBuffer commandBuffer, logicDevice* devices);


	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, logicDevice* devices);

	

	void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, logicDevice* devices);

	void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, logicDevice* devices);

	VkSampler createTextureSampler(VkPhysicalDevice device, VkDevice logicalDevice);
}