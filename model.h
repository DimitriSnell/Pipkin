#pragma once
#include"config.h"
#include"memory.h"
//#include"mesh.h"
#include"logical_device.hpp"
#include<memory>
namespace ppkin {
	class model {
	public:
		struct Vertex {
			glm::vec3 position{};
			glm::vec3 color{};
			glm::vec3 normal{};
			glm::vec2 uv{};
			int textureIndex;

			bool operator==(const Vertex& other) const {
				return position == other.position && color == other.color && normal == other.normal && uv == other.uv;
			}
		};

		struct Builder {
			std::vector<Vertex> vertices{};
			std::vector<uint32_t> indices{};
			std::vector<std::string> filepaths{};
			void loadModel(const std::string& filepath);
		};

		model(logicDevice* devices, const model::Builder &builder);
		
		void destroyModel(VkDevice device);
		void draw(VkCommandBuffer commandBuffer);
		void bind(VkCommandBuffer commandBuffer);

		static std::unique_ptr<model> createModelFromFile(logicDevice& devices, const std::string& filepath);
		Buffer buffer;
		Buffer IndexBuffer;
		bool hasIndexBuffer;
		void createTextureImage(std::string& filename);
		std::vector<VkImage> textureImages;
		std::vector<VkImageView> textureViews;
		std::vector<VkDeviceMemory> textureImageMemory;
	private:
		void createIndexBuffers(const std::vector<uint32_t> indices);
		VkCommandPool commandPool;
		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		
		logicDevice* devices;
		uint32_t vertexCount;
		uint32_t IndexCount;
		std::vector<std::string> textureFilePaths;
	};

}