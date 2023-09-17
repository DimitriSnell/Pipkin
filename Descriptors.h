#pragma once
#include"config.h"
#include<iostream>

namespace ppkin {
	struct descriptorSetLayoutData {
		int count;
		std::vector<int> indices;
		std::vector<VkDescriptorType> types;
		std::vector<int> counts;
		std::vector<VkShaderStageFlags> stages;

	};

	VkDescriptorSetLayout createDescriptorSetLayout(VkDevice device, const descriptorSetLayoutData& bindings);

	/*/ Provided by VK_VERSION_1_0
		typedef struct VkDescriptorPoolCreateInfo {
		VkStructureType                sType;
		const void* pNext;
		VkDescriptorPoolCreateFlags    flags;
		uint32_t                       maxSets;
		uint32_t                       poolSizeCount;
		const VkDescriptorPoolSize* pPoolSizes;
	} VkDescriptorPoolCreateInfo;
	*/
	VkDescriptorPool makeDescriptorPool(VkDevice device, uint32_t size, const descriptorSetLayoutData& bindings);

	VkDescriptorSet allocateDescriptorSet(VkDevice device, VkDescriptorPool descriptorPool, VkDescriptorSetLayout layout);

}