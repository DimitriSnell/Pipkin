#pragma once
#include"config.h"
#include"memory.h"
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
		void writeDescriptorSet(VkDevice device) {
			VkWriteDescriptorSet writeInfo{};
			writeInfo.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			writeInfo.dstSet = descriptorSet;
			writeInfo.dstBinding = 0;
			writeInfo.dstArrayElement = 0;
			writeInfo.descriptorCount = 1;
			writeInfo.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			writeInfo.pBufferInfo = &UBODescriptor;

			vkUpdateDescriptorSets(device,1,&writeInfo,0, nullptr);

		}
		VkDescriptorSet descriptorSet;
		VkDescriptorBufferInfo UBODescriptor;
	};
}