#pragma once
#include"config.h"
#include"memory.h"
#include<array>
namespace ppkin {
	struct UBO {
		glm::mat4 view;
		glm::mat4 projection;
		glm::mat4 viewProjection;

	};

	struct cameraData {
		UBO CData;
		Buffer cameraDataBuffer;
		void* cameraDataWriteLocation;

		void makeUBOresources(logicDevice* devices) {
			BufferInput input;
			input.devices = devices;
			input.size = sizeof(UBO);
			input.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;

			cameraDataBuffer = createBuffer(input);

			if (vkMapMemory(devices->getLogicalDevice(), cameraDataBuffer.bufferMemory, 0, sizeof(UBO), 0, &cameraDataWriteLocation) != VK_SUCCESS) {
				throw std::exception("failed to create memory for camera buffer");
			}
			else {
				std::cout << "mapped memory for camera buffer!" << std::endl;
			}
			
			UBODescriptor.buffer = cameraDataBuffer.buffer;
			UBODescriptor.offset = 0;
			UBODescriptor.range = sizeof(UBO);

		}
		void writeDescriptorSet(VkDevice device, VkDescriptorImageInfo infos[], VkSampler& sampler) {

			std::array<VkWriteDescriptorSet, 3> writeInfo;
			writeInfo[0] = {};
			writeInfo[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			writeInfo[0].dstSet = descriptorSet;
			writeInfo[0].dstBinding = 0;
			writeInfo[0].dstArrayElement = 0;
			writeInfo[0].descriptorCount = 1;
			writeInfo[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			writeInfo[0].pBufferInfo = &UBODescriptor;
			writeInfo[0].pNext = nullptr;

			VkDescriptorImageInfo samplerInfo = {};
			samplerInfo.sampler = sampler;

			writeInfo[1] = {};
			writeInfo[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			writeInfo[1].dstBinding = 1;
			writeInfo[1].dstArrayElement = 0;
			writeInfo[1].descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
			writeInfo[1].descriptorCount = 1;
			writeInfo[1].dstSet = descriptorSet;
			writeInfo[1].pBufferInfo = 0;
			writeInfo[1].pImageInfo = &samplerInfo;
			writeInfo[1].pNext = nullptr;


			writeInfo[2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			writeInfo[2].dstSet = descriptorSet;
			writeInfo[2].dstBinding = 2;
			writeInfo[2].dstArrayElement = 0;
			writeInfo[2].descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
			writeInfo[2].descriptorCount = 8;
			writeInfo[2].pBufferInfo = 0;
			writeInfo[2].pImageInfo = infos;
			writeInfo[2].pNext = nullptr;

		

			vkUpdateDescriptorSets(device,3,writeInfo.data(), 0, nullptr);

		}
		VkDescriptorSet descriptorSet;
		VkDescriptorBufferInfo UBODescriptor;
	};
}