#include "Frame_Data.h"
#include<iostream>
#include"image.h"
void ppkin::FrameData::CreateDepthResources()
{
	depthFormat = findSupportedFormat(Devices->getPhysicalDevice(), { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D24_UNORM_S8_UINT
		}, VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);

	ImageInputChunk input;
	input.physicalDevice = Devices->getPhysicalDevice();
	input.logicalDevice = Devices->getLogicalDevice();
	input.tiling = VK_IMAGE_TILING_OPTIMAL;
	input.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
	input.memoryProperties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
	input.width = width;
	input.height = height;
	input.format = depthFormat;
	depthBuffer = make_image(input);
	depthBufferMemory = makeMemory(input, depthBuffer);
	depthBufferView = makeImageViewDepth(Devices->getLogicalDevice(), depthBuffer, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);

}

void ppkin::FrameData::DestroyDepthResources()
{
	vkDestroyImage(Devices->getLogicalDevice(), depthBuffer, nullptr);
	vkFreeMemory(Devices->getLogicalDevice(), depthBufferMemory, nullptr);
	vkDestroyImageView(Devices->getLogicalDevice(), depthBufferView, nullptr);
}

void ppkin::FrameData::setDevice(logicDevice* devices)
{
	this->Devices = devices;
}

VkFormat ppkin::FrameData::findSupportedFormat(VkPhysicalDevice device, const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
{
	VkFormat result;
	bool foundFormat = false;
	for (VkFormat format : candidates) {
		VkFormatProperties properties;
		vkGetPhysicalDeviceFormatProperties(device, format, &properties);
		if (tiling == VK_IMAGE_TILING_LINEAR && (properties.linearTilingFeatures & features) == features) {
			result = format;
			foundFormat = true;
			break;
		}

		if (tiling == VK_IMAGE_TILING_OPTIMAL && (properties.optimalTilingFeatures & features) == features) {
			result = format;
			foundFormat = true;
			break;
		}

	}
	if (foundFormat) {
		return result;
	}
	else {
		throw std::runtime_error("failed to find suitable format!");
	}
}
