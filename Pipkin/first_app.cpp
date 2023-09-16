#pragma once
#include "first_app.hpp"
#include<iostream>
#include<vector>
#include<cstring>
#include<optional>
#include<set>
#include <cstdint> // Necessary for uint32_t
#include <limits> // Necessary for std::numeric_limits
#include <algorithm> 
#include <fstream>
#include"glm/gtx/string_cast.hpp"
#include"glm/gtc/constants.hpp"
namespace ppkin {
	
	void firstApp::run()
	{

		initVulkan();
		while (!ppwin.shouldClose()) {
			glfwPollEvents();
			drawFrame();
		}
		vkDeviceWaitIdle(devices.getLogicalDevice());
		cleanup();
	}
	firstApp::firstApp()
	{
		theScene = new scene();
	}
	void firstApp::initVulkan()
	{
		createInstance();
		createSurface();
		devices.setInstance(&instance);
		devices.setSurface(&surface);
		devices.pickPhysicalDevice();
		devices.createLogicalDevice();
		Schain.setWindow(&ppwin);
		Schain.createSwapChain(devices);
		createImageViews();
		createRenderPass();
		pipeline.createGraphicsPipeline(&devices, &Schain, &renderPass);
		createFramebuffers();
		createCommandPool();
		createCommandbuffer();
		createSyncObjects();
		createFrameResources();
		make_assets();
	}
	void firstApp::createInstance()
	{
		if (enableValidationLayers && !checkValidationLayerSupport()) {
			throw std::runtime_error("Validation layers requested but not available!");
		}

		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Test App";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "No Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_3;

		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;

		//glfw extensions
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		createInfo.enabledExtensionCount = glfwExtensionCount;
		createInfo.ppEnabledExtensionNames = glfwExtensions;

		//validation layers
		if (enableValidationLayers) {
			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			createInfo.ppEnabledLayerNames = validationLayers.data();
		}
		else {
			createInfo.enabledLayerCount = 0;
		}
		
		VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);

		uint32_t extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
		std::vector<VkExtensionProperties> extensions(extensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
		int size = extensions.size();
		for (int i = 0; i < size; i++) {
			
			std::cout << extensions.at(i).extensionName << std::endl;
		}

		if (result != VK_SUCCESS) {
			throw std::runtime_error("ERROR: failed to create instance");
		}
	}
	void firstApp::cleanup()
	{
		vkDestroySemaphore(devices.getLogicalDevice(), imageAvailableSemaphore, nullptr);
		vkDestroySemaphore(devices.getLogicalDevice(), renderFinishedSemaphore, nullptr);
		vkDestroyFence(devices.getLogicalDevice(), inFlightFence, nullptr);
		vkDestroyCommandPool(devices.getLogicalDevice(), commandPool, nullptr);
		for (auto framebuffer : swapChainFramebuffers) {
			vkDestroyFramebuffer(devices.getLogicalDevice(), framebuffer, nullptr);
		}
		vkDestroyDescriptorSetLayout(devices.getLogicalDevice(), pipeline.descriptorSetLayout, nullptr);
		vkDestroyPipeline(devices.getLogicalDevice(), pipeline.getPipeline(), nullptr);
		vkDestroyPipelineLayout(devices.getLogicalDevice(), pipeline.getPipelineLayout(), nullptr);
		vkDestroyRenderPass(devices.getLogicalDevice(), renderPass, nullptr);
		vkDestroySwapchainKHR(devices.getLogicalDevice(), Schain.getSwapChain(), nullptr);
		
		vkDestroySurfaceKHR(instance, surface, nullptr);
		for (auto view : swapChainImageViews) {
			vkDestroyImageView(devices.getLogicalDevice(), view, nullptr);
		}
		vkDestroyDescriptorPool(devices.getLogicalDevice(), descriptorPool, nullptr);
		for (auto cameraData : frameCameraData) {
			vkUnmapMemory(devices.getLogicalDevice(), cameraData.cameraDataBuffer.bufferMemory);
			vkFreeMemory(devices.getLogicalDevice(), cameraData.cameraDataBuffer.bufferMemory, nullptr);
			vkDestroyBuffer(devices.getLogicalDevice(), cameraData.cameraDataBuffer.buffer, nullptr);
		}
		delete meshes;
		delete theScene;
		for (auto& o : gameObjects) {
			o.ObjectModel.get()->destroyModel(devices.getLogicalDevice());
		}
		vkDestroyDevice(devices.getLogicalDevice(), nullptr);
		vkDestroyInstance(instance, nullptr);
		
	}
	bool firstApp::checkValidationLayerSupport()
	{
		bool result = true;
		uint32_t layerCount = 0;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
		std::vector<VkLayerProperties> layers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, layers.data());
		int count = 0;
		for (const char* required : validationLayers) {
			count++;
			bool foundlayer = false;
			for (const auto& available : layers) {
				if (strcmp(available.layerName, required) == 0) {
					foundlayer = true;
					break;
				}
			}
			if (foundlayer != true) {
				result = false;
			}
		}
		return result;
	}
	
	void firstApp::createImageViews()
	{
		swapChainImageViews.resize(Schain.getSwapChainImages().size());
		for (size_t i = 0; i < Schain.getSwapChainImages().size(); i++) {
			VkImageViewCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			createInfo.image = Schain.getSwapChainImages()[i];
			createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			createInfo.format = Schain.getSwapChainImageFormat();
			createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			createInfo.subresourceRange.baseMipLevel = 0;
			createInfo.subresourceRange.levelCount = 1;
			createInfo.subresourceRange.baseArrayLayer = 0;
			createInfo.subresourceRange.layerCount = 1;
			if (vkCreateImageView(devices.getLogicalDevice(), &createInfo, nullptr, &swapChainImageViews[i]) != VK_SUCCESS) {
				throw std::runtime_error("failed to create image views!");
			}
		}
	}
	/*
	void firstApp::initRayTracing()
	{
		VkPhysicalDeviceProperties2 prop2;
		prop2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
		prop2.pNext = &rayTracingProperties;
		vkGetPhysicalDeviceProperties2(physicalDevice, &prop2);
		
		
	}
	*/
	

	void firstApp::createRenderPass()
	{
		VkAttachmentDescription colorAttachment{};
		colorAttachment.format = Schain.getSwapChainImageFormat();
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference colorAttachmentRef{};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass{};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;

		VkSubpassDependency dependency{};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.srcAccessMask = 0;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		VkRenderPassCreateInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = 1;
		renderPassInfo.pAttachments = &colorAttachment;
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;

	
		if (vkCreateRenderPass(devices.getLogicalDevice(), &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS) {
			throw std::runtime_error("failed to create render pass");
		}

	}

	void firstApp::createFramebuffers()
	{
		swapChainFramebuffers.resize(swapChainImageViews.size());
		for (size_t i = 0; i < swapChainImageViews.size(); i++) {
			VkImageView attachments[] = {
				swapChainImageViews[i]
			};
			VkFramebufferCreateInfo framebufferInfo{};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = renderPass;
			framebufferInfo.attachmentCount = 1;
			framebufferInfo.pAttachments = attachments;
			framebufferInfo.width = Schain.getSwapChainExtent().width;
			framebufferInfo.height = Schain.getSwapChainExtent().height;
			framebufferInfo.layers = 1;
			if (vkCreateFramebuffer(devices.getLogicalDevice(), &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS) {
				throw std::runtime_error("failed to create frame buffer!");
			}
		}
		
	}

	void firstApp::createCommandPool()
	{
		QueueFamilyIndicies queueFamilyIndicies = devices.findQueueFamilies(devices.getPhysicalDevice());

		VkCommandPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		poolInfo.queueFamilyIndex = queueFamilyIndicies.graphicsFamily.value();

		if (vkCreateCommandPool(devices.getLogicalDevice(), &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
			throw std::runtime_error("failed to create command pool!");
		}
	}

	void firstApp::createCommandbuffer()
	{
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = commandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = 1;
		//create buffers per frame
		for (int i = 0; i < swapChainFramebuffers.size(); i++) {
			VkCommandBuffer frcmd;
			if (vkAllocateCommandBuffers(devices.getLogicalDevice(), &allocInfo, &frcmd) != VK_SUCCESS) {
				throw std::runtime_error("could not create command buffer");
			}
			else {
				frameCommandbuffers.push_back(frcmd);
				std::cout << "created command buffer for frame: " << i << std::endl;
			}
		}


		if (vkAllocateCommandBuffers(devices.getLogicalDevice(), &allocInfo, &commandBuffer) != VK_SUCCESS) {
			throw std::runtime_error("could not create command buffer");
		}
	}

	void firstApp::recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex)
	{
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = 0; // Optional
		beginInfo.pInheritanceInfo = nullptr; // Optional
		if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
			throw std::runtime_error("failed to begin recording command buffer!");
		}

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = renderPass;
		renderPassInfo.framebuffer = swapChainFramebuffers[imageIndex];
		renderPassInfo.renderArea.offset = { 0,0 };
		renderPassInfo.renderArea.extent = Schain.getSwapChainExtent();
		VkClearValue clearColor = { {{0.0f,0.0f,0.0f,1.0f}} };
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearColor;

		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);


		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.getPipeline());

		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.getPipelineLayout(), 0, 1, &frameCameraData[imageIndex].descriptorSet, 0, nullptr);
		//vkUpdateDescriptorSets(devices.getLogicalDevice(),1, frameCameraData[imageIndex].descriptorSet)
		//prepare_scene(commandBuffer);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(Schain.getSwapChainExtent().width);
		viewport.height = static_cast<float>(Schain.getSwapChainExtent().height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = Schain.getSwapChainExtent();
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);



		//int firstVertex = meshes->offsets[1]
		//TEMPORARY
		static float i = 0.0f;
		for (Object o : gameObjects) {
			
			//glm::mat4 transform = glm::translate(glm::mat4(1.0f));
			//i += .0000000000000001f;
			ObjectData objData;
			//objData.model = o.transform.mat4();
			//std::cout << glm::to_string(o.transform.mat4());
			std::cout << i << std::endl;
			o.transform.roatation.y = glm::mod(o.transform.roatation.y + 5.f,glm::two_pi<float>());
			o.transform.roatation.x = glm::mod(o.transform.roatation.x + 5.f, glm::two_pi<float>());
			//std::cout << o.transform.roatation.y << std::endl;
			objData.model = o.transform.mat4();
			objData.color = glm::vec3(1, 1, 1);
			vkCmdPushConstants(commandBuffer, pipeline.getPipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(ObjectData), &objData);
			//vkCmdDraw(commandBuffer, 3, 1, 0, 0);
			
			o.ObjectModel->bind(commandBuffer);
			o.ObjectModel->draw(commandBuffer);
			i++;
		}
		


		vkCmdEndRenderPass(commandBuffer);

		if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
			throw std::runtime_error("failed to record command buffer!");
		}
	}

	void firstApp::drawFrame()
	{
		vkWaitForFences(devices.getLogicalDevice(), 1, &inFlightFence, VK_TRUE, UINT64_MAX);
		vkResetFences(devices.getLogicalDevice(), 1, &inFlightFence);

		uint32_t imageIndex;
		vkAcquireNextImageKHR(devices.getLogicalDevice(), Schain.getSwapChain(), UINT64_MAX, imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);
		prepareFrame(imageIndex);
		vkResetCommandBuffer(frameCommandbuffers[imageIndex], 0);
		recordCommandBuffer(frameCommandbuffers[imageIndex], imageIndex);

		

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = { imageAvailableSemaphore };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;

		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &frameCommandbuffers[imageIndex];

		VkSemaphore signalSemaphores[] = { renderFinishedSemaphore };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		if (vkQueueSubmit(devices.getGraphicsQueue(), 1, &submitInfo, inFlightFence) != VK_SUCCESS) {
			throw std::runtime_error("failed to submit draw command buffer!");
		}

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;
		VkSwapchainKHR swapChains[] = { Schain.getSwapChain()};
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &imageIndex;
		
		presentInfo.pResults = nullptr; //optional

		vkQueuePresentKHR(devices.getPresentQueue(), &presentInfo);
	}

	void firstApp::createFrameResources()
	{
		descriptorSetLayoutData bindings;
		bindings.count = 1;
		bindings.types.push_back(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
		
		
		for (auto & frame : swapChainFramebuffers) {
			cameraData d;
			frameCameraData.push_back(d);
		}

		descriptorPool = makeDescriptorPool(devices.getLogicalDevice(), static_cast<uint32_t>(frameCameraData.size() + Schain.getSwapChainImages().size()), bindings);

		for (auto& frame : frameCameraData) {
			frame.makeUBOresources(&devices);
			frame.descriptorSet = allocateDescriptorSet(devices.getLogicalDevice(), descriptorPool, pipeline.descriptorSetLayout);
		}

	}

	void firstApp::createSyncObjects()
	{
		VkSemaphoreCreateInfo semaphoreInfo{};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		
		VkFenceCreateInfo fenceInfo{};

		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		if (vkCreateSemaphore(devices.getLogicalDevice(), &semaphoreInfo, nullptr, &imageAvailableSemaphore) != VK_SUCCESS ||
			vkCreateSemaphore(devices.getLogicalDevice(), &semaphoreInfo, nullptr, &renderFinishedSemaphore) != VK_SUCCESS ||
			vkCreateFence(devices.getLogicalDevice(), &fenceInfo, nullptr, &inFlightFence) != VK_SUCCESS) {
			throw std::runtime_error("failed to create semaphores!");
		}

	}

	void firstApp::make_assets()
	{
		/*
		meshes = new VertexLump();
		std::vector<float> verticies = { {
				0.0f, -0.25f, .5f, .5f, .5f,
		0.25f, 0.25f, 0.0f, 0.5f, 0.0f,
		-0.25f, 0.25f, 0.0f, 0.0f, 0.5f
	} };
		meshes->consume(verticies);
		verticies = { {
				0.1f, -0.4f, -2.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 1.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.0f, 0.0f, 1.0f

} };
		meshes->consume(verticies);

		meshes->finalize(&devices);
		*/
		/*for (float i = 5; i < 10; i += 1) {
			for (float y = 5; y < 10; y += 1) {
				std::vector<model::Vertex> verticies{
				  {{0.0f , -.5f ,0}, {1.0f, 0.0f, 0.0f}},
				  {{0.5f , .5f ,0}, {0.0f, 1.0f, 0.0f}},
				  {{-.5f , .5f ,0}, {0.0f, 0.0f, 1.0f}} };
				for (auto& v : verticies) {
					v.position.x += i;
					v.position.y += y;
				}
				auto m = std::make_shared<model>(&devices, verticies);
				Object o = Object::createObject();
				o.ObjectModel = m;
				gameObjects.push_back(o);
			}
		}*/
				/*std::vector<model::Vertex> verticies2{
				  {{3.0f, 3.5f,0}, {1.0f, 0.0f, 0.0f}},
				  {{3.5f, 3.5f,0}, {0.0f, 1.0f, 0.0f}},
				  {{3.5f, 0.5f,0}, {0.0f, 0.0f, 1.0f}} };
				model m2(&devices, verticies2);
				gameObjects.push_back(m2);
				//gameObjects.push_back(m2);*/
		std::vector<model::Vertex> verticies{

			// left face (white)
			{{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
			{{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
			{{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
			{{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
			{{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},
			{{-.5f, .5f, .5f}, {.9f, .9f, .9f}},

			// right face (yellow)
			{{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
			{{.5f, .5f, .5f}, {.8f, .8f, .1f}},
			{{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
			{{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
			{{.5f, .5f, -.5f}, {.8f, .8f, .1f}},
			{{.5f, .5f, .5f}, {.8f, .8f, .1f}},

			// top face (orange, remember y axis points down)
			{{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
			{{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
			{{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
			{{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
			{{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
			{{.5f, -.5f, .5f}, {.9f, .6f, .1f}},

			// bottom face (red)
			{{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
			{{.5f, .5f, .5f}, {.8f, .1f, .1f}},
			{{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
			{{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
			{{.5f, .5f, -.5f}, {.8f, .1f, .1f}},
			{{.5f, .5f, .5f}, {.8f, .1f, .1f}},

			// nose face (blue)
			{{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
			{{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
			{{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
			{{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
			{{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
			{{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},

			// tail face (green)
			{{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
			{{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
			{{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
			{{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
			{{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
			{{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},

		};
		for (auto& v : verticies) {
			v.position.x += 0;
			v.position.y += 0;
		};
		/*std::vector<model::Vertex> verticies{
				  {{0.0f , -.5f ,0}, {1.0f, 0.0f, 0.0f}},
				  {{0.5f , .5f ,0}, {0.0f, 1.0f, 0.0f}},
				  {{-.5f , .5f ,0}, {0.0f, 0.0f, 1.0f}} };
		*/
		auto m = std::make_shared<model>(&devices, verticies);
		Object o = Object::createObject();
		o.ObjectModel = m;
		//o.transform.translation = { .0f,.0f,.0f };
		o.transform.translation = { 0.0f,0.0f, .5f };
		o.transform.scale = { .9f,.9f,.9f };
		gameObjects.push_back(std::move(o));
	
	}

	void firstApp::prepare_scene(VkCommandBuffer commandBuffer)
	{
		//VkBuffer vertexBuffers[] = { meshes->vertexBuffer.buffer };
		//VkBuffer vertexBuffers[] = { gameObjects[0].getObjectModel().buffer.buffer };
		VkDeviceSize offset[] = { 0 };
		//vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offset);
	}

	// create projection matrix
	void firstApp::prepareFrame(uint32_t imageIndex)
	{
		static float z = -6.0f;
		z -= 0.005f;
		glm::vec3 eye = { 1.f, 0.0f, -5.f};
		glm::vec3 center = { 0.0f, 0.0f,  0.0f };
		glm::vec3 up = { 0.0f,0.0f,-1.0f };
		glm::mat4 view = glm::lookAt(eye, center, up);

		glm::mat4 projection = glm::perspective(glm::radians(45.0f), 
			(static_cast<float>(Schain.getSwapChainExtent().width) / static_cast<float>(Schain.getSwapChainExtent().height)), 0.1f, 100.0f);

		projection[1][1] *= -1;

		frameCameraData[imageIndex].CData.projection = projection;
		frameCameraData[imageIndex].CData.view = view;
		//frameCameraData[imageIndex].CData.projection = glm::mat4(1.0);
		//frameCameraData[imageIndex].CData.view = glm::mat4(1.0);
		//std::cout << glm::to_string(projection) << glm::to_string(view) << glm::to_string((view * projection)) << std::endl;
		frameCameraData[imageIndex].CData.viewProjection = view * projection;
		//frameCameraData[imageIndex].CData.viewProjection = glm::mat4(1.0);
		memcpy(frameCameraData[imageIndex].cameraDataWriteLocation, &frameCameraData[imageIndex].CData, sizeof(UBO));

		frameCameraData[imageIndex].writeDescriptorSet(devices.getLogicalDevice());
	}
	
	void firstApp::createSurface()
	{
		if (glfwCreateWindowSurface(instance, ppwin.getWindow(), nullptr, &surface) != VK_SUCCESS) {
			throw std::runtime_error("failed to create window surface!");
		}
	}

	VkShaderModule firstApp::createShaderModule(const std::vector<char>& code)
	{
		VkShaderModule shaderModule;
		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());
		
		if (vkCreateShaderModule(devices.getLogicalDevice(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
			throw std::runtime_error("failed to create shader module!");
		}

		return shaderModule;
	}

	std::vector<char> firstApp::readFile(const std::string& filename)
	{
		std::ifstream file(filename, std::ios::ate | std::ios::binary);

		if (!file.is_open()) {
			throw std::runtime_error("failed to open file!");
		}
		size_t fileSize = (size_t)file.tellg();
		std::vector<char> buffer(fileSize);
		file.seekg(0);
		file.read(buffer.data(), fileSize);
		file.close();
		std::cout << buffer.size() << std::endl;
		return buffer;
	}
	
}

