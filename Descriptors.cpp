#pragma once
#include "Descriptors.h"

VkDescriptorSetLayout ppkin::createDescriptorSetLayout(VkDevice device, const descriptorSetLayoutData& bindings)
{
	VkDescriptorSetLayout result;
	std::vector<VkDescriptorSetLayoutBinding> layoutBindings;
	layoutBindings.reserve(bindings.count);
	for (int i = 0; i < bindings.count; i++) {
		VkDescriptorSetLayoutBinding binding;
		binding.binding = bindings.indices[i];
		binding.descriptorType = bindings.types[i];
		binding.descriptorCount = bindings.counts[i];
		binding.stageFlags = bindings.stages[i];
		layoutBindings.push_back(binding);

	}
	
	VkDescriptorSetLayoutBinding samplerLayoutBinding{};
	samplerLayoutBinding.binding = 1;
	samplerLayoutBinding.descriptorCount = 1;
	samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
	samplerLayoutBinding.pImmutableSamplers = nullptr;
	samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	layoutBindings.push_back(samplerLayoutBinding);

	VkDescriptorSetLayoutBinding arrayLayoutBinding{};
	arrayLayoutBinding.binding = 2;
	arrayLayoutBinding.descriptorCount = 8;
	arrayLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
	arrayLayoutBinding.pImmutableSamplers = nullptr;
	arrayLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	layoutBindings.push_back(arrayLayoutBinding);

	VkDescriptorSetLayoutCreateInfo layoutInfo;
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = static_cast<uint32_t>(layoutBindings.size());
	layoutInfo.pBindings = layoutBindings.data();
	layoutInfo.flags = 0;
	layoutInfo.pNext = nullptr;


	if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &result) != VK_SUCCESS) {
		throw std::exception("could not create descriptor set layout!");
	}
	else {
		std::cout << "created descriptor set layout!" << std::endl;
	}

	return result;
}

VkDescriptorPool ppkin::makeDescriptorPool(VkDevice device, uint32_t size, const descriptorSetLayoutData& bindings)
{
	std::vector<VkDescriptorPoolSize> poolSizes;
	VkDescriptorPool result;
	for (int i = 0; i < bindings.count; i++) {
		VkDescriptorPoolSize poolSize;
		poolSize.type = bindings.types[i];
		poolSize.descriptorCount = size;
		poolSizes.push_back(poolSize);
	}

	VkDescriptorPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	poolInfo.maxSets = size;
	poolInfo.pPoolSizes = poolSizes.data();
	poolInfo.pNext = nullptr;

	if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &result) != VK_SUCCESS) {
		throw std::exception("failed to create descriptor pool!");
	}
	else {
		std::cout << "Created descirptor pool!" << std::endl;
	}
	return result;
}

VkDescriptorSet ppkin::allocateDescriptorSet(VkDevice device, VkDescriptorPool descriptorPool, VkDescriptorSetLayout layout)
{
	VkDescriptorSet result;
	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorPool;
	allocInfo.descriptorSetCount = 1;
	allocInfo.pSetLayouts = &layout;
	allocInfo.pNext = nullptr;

	if (vkAllocateDescriptorSets(device, &allocInfo,&result) != VK_SUCCESS) {
		throw std::exception("failed to allocate descriptor set!");
	}
	else {
		std::cout << "Allocated descriptor set!" << std::endl;
	}
	return result;
}
