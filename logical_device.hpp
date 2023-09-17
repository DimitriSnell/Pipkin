#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>
#include<fstream>
#include<vector>
#include<optional>
struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};
struct QueueFamilyIndicies {
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;
	bool isComplete() {
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};
namespace ppkin {
	class logicDevice {
	public:
		void logicalDevice();
		const VkDevice& getLogicalDevice();
		const VkPhysicalDevice& getPhysicalDevice();
		QueueFamilyIndicies findQueueFamilies(VkPhysicalDevice device);
		void setInstance(VkInstance* newInstance);
		void setSurface(VkSurfaceKHR* newsurface);
		void createLogicalDevice();
		void pickPhysicalDevice();
		SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
		const VkSurfaceKHR& getSurface();
		const VkQueue& getGraphicsQueue();
		const VkQueue& getPresentQueue();
	private:
		bool isSuitableDevice(VkPhysicalDevice device);
		bool checkDeviceExtensionSupport(VkPhysicalDevice device);

		const std::vector<const char*> deviceExtensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME, VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME,
			VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME,VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME, VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME,
			VK_KHR_RAY_QUERY_EXTENSION_NAME
		};

		VkDevice device;
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
		VkInstance* instance;
		VkSurfaceKHR* surface;
		VkQueue Gqueue;
		VkQueue Pqueue;
	};

}