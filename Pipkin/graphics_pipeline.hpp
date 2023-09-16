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

		
		VkAttachmentDescription makeDepthAttachment(VkFormat depthFormat) {
			VkAttachmentDescription depthAttachment{};
			//depthAttachment.flags = VK_ATTACHMENT_DESCRIPTION_FLAG_BITS_MAX_ENUM;
			depthAttachment.format = depthFormat;
			depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
			depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
			return depthAttachment;

		}
		VkAttachmentReference makeDepthAttachmentReference() {
			VkAttachmentReference depthAttachmentRef{};
			depthAttachmentRef.attachment = 1;
			depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
			return depthAttachmentRef;
		}
	private:
		void makeDescriptorSetLayout();
		VkShaderModule createShaderModule(const std::vector<char>& code);
		std::vector<char> readFile(const std::string& filename);
		VkPipelineLayout pipelineLayout;
		VkPipeline pipeline;
		logicDevice* devices;
	};
}