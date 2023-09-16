#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>
#include<fstream>
#include<vector>
#include"logical_device.hpp"
#include"swap_chain.h"
#include"render_structs.h"
namespace ppkin {

	class graphicsPipeline {
	public:
		graphicsPipeline();
		~graphicsPipeline();
		VkPipeline& getPipeline();
		VkPipelineLayout& getPipelineLayout();
		void setDevice(logicDevice* device);
		void createGraphicsPipeline(logicDevice* device, swapChain* Schain, VkRenderPass* renderPass);
		VkDescriptorSetLayout descriptorSetLayout;
	private:
		void makeDescriptorSetLayout();
		VkShaderModule createShaderModule(const std::vector<char>& code);
		std::vector<char> readFile(const std::string& filename);
		VkPipelineLayout pipelineLayout;
		VkPipeline pipeline;
		logicDevice* devices;
	};
}