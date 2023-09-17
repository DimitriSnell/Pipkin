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

	VkImageView makeImageViewDepth(VkDevice logicalDevice, VkImage image, VkFormat format, VkImageAspectFlags aspect) {

		VkImageView result;
		VkImageViewCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = image;
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = format;
		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.subresourceRange.aspectMask = aspect;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;

		if (vkCreateImageView(logicalDevice, &createInfo, nullptr, &result) != VK_SUCCESS) {
			throw std::exception("failed to create image view!");
		}
		else {
			return result;
		}


	}

	VkImage make_image(ImageInputChunk input) {
		/*
		typedef struct VkImageCreateInfo {
			VkStructureType          sType;
			const void* pNext;
			VkImageCreateFlags       flags;
			VkImageType              imageType;
			VkFormat                 format;
			VkExtent3D               extent;
			uint32_t                 mipLevels;
			uint32_t                 arrayLayers;
			VkSampleCountFlagBits    samples;
			VkImageTiling            tiling;
			VkImageUsageFlags        usage;
			VkSharingMode            sharingMode;
			uint32_t                 queueFamilyIndexCount;
			const uint32_t* pQueueFamilyIndices;
			VkImageLayout            initialLayout;
		} VkImageCreateInfo;
		*/
		VkImage result;
		VkImageCreateInfo imageInfo;
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.flags = VkImageCreateFlagBits();
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		VkExtent3D extent;
		extent.width = input.width;
		extent.height = input.height;
		extent.depth = 1;
		imageInfo.extent = extent;
		imageInfo.mipLevels = 1;
		imageInfo.arrayLayers = 1;
		imageInfo.format = input.format;
		imageInfo.tiling = input.tiling;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.usage = input.usage;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageInfo.pNext = nullptr;
		
		if (vkCreateImage(input.logicalDevice, &imageInfo, nullptr, &result) != VK_SUCCESS) {
			throw std::exception("failed to create image!");
		}
		
		return result;
	}


	VkDeviceMemory makeMemory(ImageInputChunk input, VkImage image) {
		VkDeviceMemory result;
		VkMemoryRequirements requirements;
		vkGetImageMemoryRequirements(input.logicalDevice, image, &requirements);

		VkMemoryAllocateInfo allocation;
		allocation.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocation.allocationSize = requirements.size;
		allocation.memoryTypeIndex = findMemoryTypeIndex(
			input.physicalDevice, requirements.memoryTypeBits, input.memoryProperties
		);
		allocation.pNext = nullptr;
		// = input.logicalDevice.allocateMemory(allocation);
		if (vkAllocateMemory(input.logicalDevice, &allocation, nullptr, &result) != VK_SUCCESS) {
			throw std::exception("failed to bind image memory!");
		}
		//input.logicalDevice.bindImageMemory(image, imageMemory, 0);
		if (vkBindImageMemory(input.logicalDevice, image, result, 0) != VK_SUCCESS) {
			throw std::exception("failed to bind image memory!");
		}
		else {
			return result;
		}

	}

	
}