#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>
#include<fstream>
#include<vector>
#include<optional>
#include"logical_device.hpp"
#include"pp_window.hpp"
namespace ppkin {
	class swapChain {
	public:
		void createSwapChain(logicDevice& devices);
		VkSwapchainKHR& getSwapChain();
		void setWindow(ppWindow* window);
		const std::vector<VkImage>& getSwapChainImages();
		const VkFormat& getSwapChainImageFormat();
		const VkExtent2D& getSwapChainExtent();
	private:
		VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
		VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
		VkSwapchainKHR Schain;
		ppWindow* ppwin;
		std::vector <VkImage> swapChainImages;
		VkFormat swapChainImageFormat;
		VkExtent2D swapChainExtent;
	};
}