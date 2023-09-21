#pragma once
#include "model.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include<tiny_obj_loader.h>
#include<iostream>
#include"utils.h"
#define GLM_ENABLE_EXPERIMENTAL
#include<glm/gtx/hash.hpp>
#include<unordered_map>
#define STB_IMAGE_IMPLEMENTATION
#include<stb_image.h>
#include"image.h"
namespace std {
	template<>
	struct hash<ppkin::model::Vertex> {
		size_t operator()(ppkin::model::Vertex const& vertex) const {
			size_t seed = 0;
			ppkin::hashCombine(seed, vertex.position, vertex.color, vertex.normal, vertex.uv);
			return seed;
		}
	};
}
namespace ppkin {
	model::model(logicDevice* devices, const model::Builder& builder)
	{
		this->devices = devices;
		BufferInput inputChunk;
		inputChunk.devices = devices;
		vertexCount = static_cast<uint32_t>(builder.vertices.size());
		inputChunk.size = sizeof(builder.vertices[0]) * vertexCount;
		inputChunk.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		buffer = createBuffer(inputChunk);
		void* memoryLocation;
		if (vkMapMemory(devices->getLogicalDevice(), buffer.bufferMemory, 0, inputChunk.size, 0, &memoryLocation) != VK_SUCCESS) {
			throw std::exception("failed to map memory!");
		}
		else {
			std::cout << ("successfully mapped memory") << std::endl;
		}
		memcpy(memoryLocation, builder.vertices.data(), static_cast<size_t>(inputChunk.size));
		vkUnmapMemory(devices->getLogicalDevice(), buffer.bufferMemory);

		createIndexBuffers(builder.indices);

		this->textureFilePaths = builder.filepaths;
		for (int i = 0; i < textureFilePaths.size(); i++) {
			std::string name = textureFilePaths[i] + ".png";
			model::createTextureImage(name);
		}
		//createTextureImage();
	}

	void model::createTextureImage(std::string& filename)
	{
		std::cout << filename << std::endl;
		int texWidth, texHeight, texChannels;
		stbi_uc* pixels = stbi_load(filename.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
		VkDeviceSize imageSize = texWidth * texHeight * 4;

		if (!pixels) {
			std::cout << "failed to find texture using default" << std::endl;
			return;
			//throw std::runtime_error("failed to load texture image!");
		}
		else {
			std::cout << "found texture!" << std::endl;
			
			Buffer tempBuffer;


			BufferInput inputChunk;
			inputChunk.devices = devices;
			inputChunk.size = imageSize;
			inputChunk.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
			tempBuffer = createBuffer(inputChunk);

			stagingBuffer = tempBuffer.buffer;
			stagingBufferMemory = tempBuffer.bufferMemory;
			void* data;
			vkMapMemory(devices->getLogicalDevice(), stagingBufferMemory, 0, imageSize, 0, &data);
			memcpy(data, pixels, static_cast<size_t>(imageSize));
			vkUnmapMemory(devices->getLogicalDevice(), stagingBufferMemory);

			/*VkDevice logicalDevice;
			VkPhysicalDevice physicalDevice;
			int width;
			int height;
			VkImageTiling tiling;
			VkImageUsageFlags usage;
			VkMemoryPropertyFlags memoryProperties;
			VkFormat format;*/

			ImageInputChunk input;
			input.logicalDevice = devices->getLogicalDevice();
			input.physicalDevice = devices->getPhysicalDevice();
			input.width = static_cast<uint32_t>(texWidth);
			input.height = static_cast<uint32_t>(texHeight);
			input.tiling = VK_IMAGE_TILING_OPTIMAL;
			input.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
			input.format = VK_FORMAT_R8G8B8A8_SRGB;
			input.memoryProperties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
			VkImage image = make_image(input);
			textureImages.push_back(image);
			VkDeviceMemory memory = makeMemory(input, image);
			textureImageMemory.push_back(memory);
			transitionImageLayout(textureImages[textureImages.size() - 1], VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,devices);
			copyBufferToImage(stagingBuffer, textureImages[textureImages.size() - 1], static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight),devices);
			transitionImageLayout(textureImages[textureImages.size() - 1], VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,devices);
			VkImageView view = makeImageViewDepth(devices->getLogicalDevice(), image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);
			textureViews.push_back(view);
		}
	}





	void model::destroyModel(VkDevice device)
	{
		vkDestroyBuffer(device, buffer.buffer, nullptr);
		vkFreeMemory(device, buffer.bufferMemory, nullptr);
		if (hasIndexBuffer) {
			vkDestroyBuffer(device, IndexBuffer.buffer, nullptr);
			vkFreeMemory(device, IndexBuffer.bufferMemory, nullptr);
		}
	}
	void model::draw(VkCommandBuffer commandBuffer)
	{
		if (hasIndexBuffer) {
			vkCmdDrawIndexed(commandBuffer, IndexCount, 1, 0, 0, 0);
		}
		else {
			vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
		}
	}
	void model::bind(VkCommandBuffer commandBuffer)
	{
		//VkBuffer vertexBuffers[] = { meshes->vertexBuffer.buffer };
		VkBuffer vertexBuffers[] = { buffer.buffer };
		VkDeviceSize offset[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offset);

		if (hasIndexBuffer) {
			vkCmdBindIndexBuffer(commandBuffer, IndexBuffer.buffer, 0, VK_INDEX_TYPE_UINT32);
		}
	}

	void model::createIndexBuffers(const std::vector<uint32_t> indices)
	{
		BufferInput inputChunk;
		inputChunk.devices = devices;
		IndexCount = static_cast<uint32_t>(indices.size());
		hasIndexBuffer = IndexCount > 0;
		if (!hasIndexBuffer) {
			return;
		}
		inputChunk.size = sizeof(indices[0]) * IndexCount;
		inputChunk.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
		IndexBuffer = createBuffer(inputChunk);
		void* memoryLocation;
		if (vkMapMemory(devices->getLogicalDevice(), IndexBuffer.bufferMemory, 0, inputChunk.size, 0, &memoryLocation) != VK_SUCCESS) {
			throw std::exception("failed to map memory!");
		}
		else {
			std::cout << ("successfully mapped memory") << std::endl;
		}
		memcpy(memoryLocation, indices.data(), static_cast<size_t>(inputChunk.size));
		vkUnmapMemory(devices->getLogicalDevice(), IndexBuffer.bufferMemory);
	}

	

	



	std::unique_ptr<model> model::createModelFromFile(logicDevice& devices, const std::string& filepath)
	{
		Builder builder{};
		builder.loadModel(filepath);
		auto m = std::make_unique<model>(&devices, builder);
		std::cout << "vertex count: " << builder.vertices.size() << std::endl;
		return m;
	}

	void model::Builder::loadModel(const std::string& filepath)
	{
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string warn;
		std::string error;
		std::unordered_map<Vertex, uint32_t> uniqueVertices{};
		
		if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &error, filepath.c_str())) {
			std::cout << warn << error << std::endl;
			throw std::runtime_error(warn + error);
		}

		vertices.clear();
		indices.clear();
		int i = 0;
		for (const auto& shape : shapes) {
			for (const auto& index : shape.mesh.indices) {
				Vertex vertex{};
				if (index.vertex_index >= 0) {
					vertex.position = { attrib.vertices[3 * index.vertex_index + 0],
					attrib.vertices[3 * index.vertex_index + 1],
					attrib.vertices[3 * index.vertex_index + 2]
					};

					vertex.color = { attrib.colors[3 * index.vertex_index + 0],
					attrib.colors[3 * index.vertex_index + 1],
					attrib.colors[3 * index.vertex_index + 2]
					};

				}
				if (index.normal_index >= 0) {
					vertex.normal = { attrib.normals[3 * index.normal_index + 0],
					attrib.normals[3 * index.normal_index + 1],
					attrib.normals[3 * index.normal_index + 2]
					};
				}
				if (index.texcoord_index >= 0) {
					vertex.uv = {
						attrib.texcoords[2 * index.texcoord_index + 0],
						1.0f - attrib.texcoords[2 * index.texcoord_index + 1]

					};
					
				}
				vertex.textureIndex = i;
				if (uniqueVertices.count(vertex) == 0) {
					uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
					vertices.push_back(vertex);
				}
				indices.push_back(uniqueVertices[vertex]);
				//std::cout << i << std::endl;
			}
			std::string texFilePath = ("C:/Users/meach/Desktop/2bModel/textures/" + shape.name);
			filepaths.push_back(texFilePath);
			std::cout << shapes[i].name.c_str() << " " << i <<std::endl;
			i++;

		}
		//for (int i = 0; i < shapes.size(); i++) {
			//std::cout << shapes[i].name.c_str() << std::endl;
		//}
		
		tinyobj::ObjReaderConfig reader_config;
		reader_config.mtl_search_path = "C:/Users/meach/Desktop/model/"; // Path to material files
		
		tinyobj::ObjReader reader;

		if (!reader.ParseFromFile(filepath, reader_config)) {
			if (!reader.Error().empty()) {
				std::cerr << "TinyObjReader: " << reader.Error();
			}
			exit(1);
		}

		if (!reader.Warning().empty()) {
			std::cout << "TinyObjReader: " << reader.Warning();
		}
		materials = reader.GetMaterials();

		/*for (int i = 0; i < materials.size(); i++) {
			std::cout << materials[i].name.c_str() << std::endl;
		}*/
	}
	
}
