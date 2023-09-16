#pragma once
#include "swap_chain.h"
#include<algorithm>
void ppkin::swapChain::createSwapChain(logicDevice& devices)
{
	SwapChainSupportDetails swapChainSupport = devices.querySwapChainSupport(devices.getPhysicalDevice());


	VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
	VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
	VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

	uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
	if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
		imageCount = swapChainSupport.capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = devices.getSurface();

	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	//QueueFamilyIndicies indices = findQueueFamilies(physicalDevice);
	QueueFamilyIndicies indices = devices.findQueueFamilies(devices.getPhysicalDevice());
	uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };
	if (indices.graphicsFamily != indices.presentFamily) {
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else {
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0;
		createInfo.pQueueFamilyIndices = nullptr;
	}
	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;
	createInfo.oldSwapchain = VK_NULL_HANDLE;

	if (vkCreateSwapchainKHR(devices.getLogicalDevice(), &createInfo, nullptr, &Schain) != VK_SUCCESS) {
		throw std::runtime_error("failed to create swap chain!");
	}

	vkGetSwapchainImagesKHR(devices.getLogicalDevice(), Schain, &imageCount, nullptr);
	swapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(devices.getLogicalDevice(), Schain, &imageCount, swapChainImages.data());
	swapChainImageFormat = surfaceFormat.format;
	swapChainExtent = extent;
}

VkSwapchainKHR& ppkin::swapChain::getSwapChain()
{
	return Schain;
}

void ppkin::swapChain::setWindow(ppWindow* window)
{
	ppwin = window;
}

const std::vector<VkImage>& ppkin::swapChain::getSwapChainImages()
{
	return swapChainImages;
}

const VkFormat& ppkin::swapChain::getSwapChainImageFormat()
{
	return swapChainImageFormat;
}

const VkExtent2D& ppkin::swapChain::getSwapChainExtent()
{
	return swapChainExtent;
}

VkSurfaceFormatKHR ppkin::swapChain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
	VkSurfaceFormatKHR format = availableFormats[0];

	for (const auto& available : availableFormats) {
		if (available.format == VK_FORMAT_B8G8R8A8_SRGB && available.colorSpace == VK_COLORSPACE_SRGB_NONLINEAR_KHR) {
			format = available;
		}
	}

	return format;
}

VkPresentModeKHR ppkin::swapChain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
{
	VkPresentModeKHR mode = VK_PRESENT_MODE_FIFO_KHR;
	for (const auto& available : availablePresentModes) {
		if (available == VK_PRESENT_MODE_MAILBOX_KHR) {
			mode = VK_PRESENT_MODE_MAILBOX_KHR;
			break;
		}
	}
	return mode;
}

VkExtent2D ppkin::swapChain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
{
	VkExtent2D result;
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
		result = capabilities.currentExtent;
	}
	else {
		int width;
		int height;
		glfwGetFramebufferSize(ppwin->getWindow(), &width, &height);

		VkExtent2D actualExtent = {
			 static_cast<uint32_t>(width),
			static_cast<uint32_t>(height)
		};
		actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
		actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
		result = actualExtent;
	}
	return result;
}
