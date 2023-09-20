#pragma once
#include "model.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include<tiny_obj_loader.h>
#include<iostream>
#include"utils.h"
#define GLM_ENABLE_EXPERIMENTAL
#include<glm/gtx/hash.hpp>
#include<unordered_map>

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
						attrib.texcoords[2* index.texcoord_index + 0],
						attrib.texcoords[2 * index.texcoord_index + 1]

					};
				}
				if (uniqueVertices.count(vertex) == 0) {
					uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
					vertices.push_back(vertex);
				}
				indices.push_back(uniqueVertices[vertex]);
			}
		}
	}

}
