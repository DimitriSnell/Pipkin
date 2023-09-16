#pragma once
#include "logical_device.hpp"
#include<set>
void ppkin::logicDevice::logicalDevice()
{
	
}

const VkDevice& ppkin::logicDevice::getLogicalDevice()
{
	return device;
}

const VkPhysicalDevice& ppkin::logicDevice::getPhysicalDevice()
{
	return physicalDevice;
}

QueueFamilyIndicies ppkin::logicDevice::findQueueFamilies(VkPhysicalDevice device)
{
	QueueFamilyIndicies indicies;

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());
	int i = 0;
	for (const auto& family : queueFamilies) {
		if (family.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			indicies.graphicsFamily = i;
		}
		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, *surface, &presentSupport);
		if (presentSupport) {
			indicies.presentFamily = i;
		}
		if (indicies.isComplete()) {
			break;
		}
		i++;
	}


	return indicies;
}

void ppkin::logicDevice::setInstance(VkInstance* newInstance)
{
	instance = newInstance;
}

void ppkin::logicDevice::setSurface(VkSurfaceKHR* newsurface)
{
	surface = newsurface;
}



void ppkin::logicDevice::createLogicalDevice()
{
	QueueFamilyIndicies indices = findQueueFamilies(physicalDevice);
	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };
	float queuePriority = 1.0f;
	for (uint32_t queueFamily : uniqueQueueFamilies) {
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	VkPhysicalDeviceFeatures deviceFeatures{};

	VkPhysicalDeviceRayQueryFeaturesKHR qFeatures = {};
	qFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_QUERY_FEATURES_KHR;
	qFeatures.rayQuery = VK_TRUE;


	VkPhysicalDeviceVulkan12Features vul12Features = {};
	vul12Features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES;
	vul12Features.descriptorIndexing = VK_TRUE;
	vul12Features.bufferDeviceAddress = VK_TRUE;
	vul12Features.pNext = &qFeatures;

	VkPhysicalDeviceAccelerationStructureFeaturesKHR asFeatures = {};
	asFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR;
	asFeatures.accelerationStructure = VK_TRUE;
	asFeatures.pNext = &vul12Features;

	VkPhysicalDeviceRayTracingPipelineFeaturesKHR pipeFeatures = {};
	pipeFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_FEATURES_KHR;
	pipeFeatures.rayTracingPipeline = VK_TRUE;
	pipeFeatures.pNext = &asFeatures;

	//const char* enabledEXtensions[5] = { "VK_KHR_ray_tracing_pipeline", "VK_KHR_accelration_structure", "VK_EXT_descriptor_indexing", "VK_KHR_deferred_host_operations" };

	VkDeviceCreateInfo createInfo{};
	createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
	createInfo.ppEnabledExtensionNames = deviceExtensions.data();
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.pQueueCreateInfos = queueCreateInfos.data();

	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());;
	createInfo.pEnabledFeatures = &deviceFeatures;

	createInfo.pNext = &pipeFeatures;



	if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
		throw std::runtime_error("failed to create logical device!");
	}
	vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &Gqueue);
	vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &Pqueue);
}

void ppkin::logicDevice::pickPhysicalDevice()
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(*instance, &deviceCount, nullptr);
	std::vector<VkPhysicalDevice> devices(deviceCount);
	if (deviceCount == 0) {
		throw std::runtime_error("failed to find GPUs with Vulkan support!");
	}
	vkEnumeratePhysicalDevices(*instance, &deviceCount, devices.data());
	//std::vector < VkPhysicalDeviceProperties> properties(deviceCount);

	for (const auto& device : devices) {
		if (isSuitableDevice(device)) {
			physicalDevice = device;
			break;
		}
	}
	if (physicalDevice == VK_NULL_HANDLE) {
		throw std::runtime_error("failed to find a suitable GPU!");
	}
}

SwapChainSupportDetails ppkin::logicDevice::querySwapChainSupport(VkPhysicalDevice device)
{
	SwapChainSupportDetails details;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, *surface, &details.capabilities);
	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, *surface, &formatCount, nullptr);
	if (formatCount != 0)
	{
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, *surface, &formatCount, details.formats.data());
	}
	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, *surface, &presentModeCount, nullptr);

	if (presentModeCount != 0) {
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, *surface, &presentModeCount, details.presentModes.data());
	}

	return details;
}

const VkSurfaceKHR& ppkin::logicDevice::getSurface()
{
	return *surface;
}

const VkQueue& ppkin::logicDevice::getGraphicsQueue()
{
	return Gqueue;
}

const VkQueue& ppkin::logicDevice::getPresentQueue()
{
	return Pqueue;
}



bool ppkin::logicDevice::isSuitableDevice(VkPhysicalDevice device)
{
	bool result;
	QueueFamilyIndicies indicies = findQueueFamilies(device);
	VkPhysicalDeviceProperties properties;
	VkPhysicalDeviceFeatures features;
	vkGetPhysicalDeviceProperties(device, &properties);
	vkGetPhysicalDeviceFeatures(device, &features);
	std::cout << properties.deviceName << std::endl;

	bool extensionsSupported = checkDeviceExtensionSupport(device);
	std::cout << extensionsSupported << std::endl;

	bool swapChainAdequate = false;
	if (extensionsSupported) {
		SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
		swapChainAdequate = (!swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty());
	}

	result = ((properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
		features.geometryShader) && indicies.isComplete() && swapChainAdequate);
	return result;
}

bool ppkin::logicDevice::checkDeviceExtensionSupport(VkPhysicalDevice device)
{
	bool result = false;
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
	std::vector<VkExtensionProperties> supportedExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, supportedExtensions.data());
	int count = 0;
	for (const auto& required : deviceExtensions) {

		for (const auto& extension : supportedExtensions) {
			//std::cout << extension.extensionName << std::endl;
			if (std::strcmp(required, extension.extensionName) == true) {
				count++;
				break;
			}
		}
	}
	if (count == deviceExtensions.size()) {
		result = true;
	}
	return result;
}


