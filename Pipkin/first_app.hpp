#pragma once
#include "pp_window.hpp"
#include"logical_device.hpp"
#include<vector>
#include<optional>
#include"swap_chain.h"
#include"graphics_pipeline.hpp"
#include"triangle_mesh.h"
#include"vertex_lump.h"
#include"config.h"
#include"scene.h"
#include"UBO.h"
#include"Descriptors.h"
#include"object.h"
#include"Frame_Data.h"
namespace ppkin {
	class firstApp {
	public:
		static constexpr int WIDTH = 1500;
		static constexpr int HEIGHT = 1200;
		void run();
		firstApp();

	private:
		void initVulkan();
		void createInstance();
		void cleanup();
		bool checkValidationLayerSupport();
		void createSurface();
		void createImageViews();
		//void initRayTracing();
		void createRenderPass();
		void createFramebuffers();
		void createCommandPool();
		void createCommandbuffer();
		void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
		void drawFrame();
		void createFrameResources();
		void createSyncObjects();
		void make_assets();
		void prepare_scene(VkCommandBuffer commandBuffer);
		void prepareFrame(uint32_t imageIndex);
		//QueueFamilyIndicies findQueueFamilies(VkPhysicalDevice device);
		VkShaderModule createShaderModule(const std::vector<char>& code);
		const std::vector<const char*> validationLayers = {
			"VK_LAYER_KHRONOS_validation", 
		};
		const std::vector<const char*> deviceExtensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME, VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME, 
			VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME,VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME, VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME,
			VK_KHR_RAY_QUERY_EXTENSION_NAME
		};
		const bool enableValidationLayers = true;
		VkInstance instance;
		ppWindow ppwin{ WIDTH, HEIGHT, "pipkin" };
		VkSurfaceKHR surface;
		std::vector<VkImageView> swapChainImageViews;
		VkPhysicalDeviceRayTracingPipelinePropertiesKHR rayTracingProperties{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_PROPERTIES_KHR };
		static std::vector<char> readFile(const std::string& filename);
		VkRenderPass renderPass;
		//VkPipelineLayout pipelineLayout;//
		
		// one in each vector for each frame in the swap chain
		std::vector<VkFramebuffer> swapChainFramebuffers;
		std::vector<cameraData> frameCameraData;
		std::vector<VkCommandBuffer> frameCommandbuffers;
		std::vector<FrameData> depthBufferData;

		VkDescriptorPool descriptorPool;
		VkCommandPool commandPool;
		VkCommandBuffer commandBuffer;
		VkSemaphore imageAvailableSemaphore;
		VkSemaphore renderFinishedSemaphore;
		VkFence inFlightFence;
		logicDevice devices;
		swapChain Schain;
		graphicsPipeline pipeline;

		std::vector<Object> gameObjects;

		scene* theScene;

		VertexLump* meshes;
		//TriangleMesh* triangleMesh;

	};

}