#pragma once
#include"config.h"
#include"logical_device.hpp"
namespace ppkin {
	class FrameData {
	public:
		void CreateDepthResources();
		void DestroyDepthResources();
		void setDevice(logicDevice* devices);
		int width;
		int height;
		VkFormat depthFormat;
		VkImageView depthBufferView;
	private:
		VkImage depthBuffer;
		VkDeviceMemory depthBufferMemory;
		
		logicDevice* Devices;
		
		

		VkFormat findSupportedFormat(VkPhysicalDevice device, const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);


	};
}