#include "LogicalDevice.h"


namespace VK{
void LogicalDevice::Create (const PhysicalDevice& physicalDevice, VkAllocationCallbacks* allocator){
	this->physicalDevice = physicalDevice;
	this->allocator = allocator;
	std::vector<QueueFamilyIndex> indices = physicalDevice.GetQueueFamilyIndices ();
	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos (indices.size ());
	U16 iterator = 0;
	for(const auto& queueFamily : physicalDevice.GetQueueFamilyIndices ()){
		FillQueueCreateInfo (queueFamily, queueCreateInfos[iterator++]);
	}
	VkDeviceCreateInfo createInfo = {};
	FillDeviceCreateInfo (queueCreateInfos, createInfo);

	if(vkCreateDevice (physicalDevice.GetPhysicalDevice (), &createInfo, allocator, &logicalDevice) != VK_SUCCESS){
		ERROR ("failed to create logical device!");
	}

	if(logicalDevice == VK_NULL_HANDLE)
		ERROR ("Failed to create logica device");
	LIE::Debug::Print ("Logical device created");
}

void LogicalDevice::Destroy (){
	if(logicalDevice != VK_NULL_HANDLE){
		vkDestroyDevice (logicalDevice, allocator);
		logicalDevice = VK_NULL_HANDLE;
		PRINT ("Logical device destroyed");
	}
}

const VkDevice& LogicalDevice::GetLogicalDevice () const{
	return logicalDevice;
}

const PhysicalDevice & LogicalDevice::GetPhysicalDevice () const{
	return physicalDevice;
}

void LogicalDevice::FillQueueCreateInfo (const QueueFamilyIndex& queueFamilyIndex, VkDeviceQueueCreateInfo& queueCreateInfo){
	std::vector<QueueFamilyIndex> indices = physicalDevice.GetQueueFamilyIndices ();
	if(indices.size () == 0){
		ERROR ("No queue index has found in physical device");
	}
	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfo.queueFamilyIndex = queueFamilyIndex.queueFamily.value();
	queueCreateInfo.queueCount = queueFamilyIndex.queueCount;
	queueCreateInfo.pQueuePriorities = &queueFamilyIndex.priority;
}

void LogicalDevice::FillDeviceCreateInfo (const std::vector<VkDeviceQueueCreateInfo>& queueCreateInfos, VkDeviceCreateInfo& createInfo){
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.pQueueCreateInfos = queueCreateInfos.data();
	createInfo.queueCreateInfoCount = static_cast<U32>(queueCreateInfos.size());
	createInfo.pEnabledFeatures = &physicalDevice.GetEnabledFeatures();
	createInfo.enabledExtensionCount = static_cast<U32>(physicalDevice.GetDeviceExtensions().size());
	createInfo.ppEnabledExtensionNames = physicalDevice.GetDeviceExtensions ().data ();
	if(IS_DEBUG){
		createInfo.enabledLayerCount = static_cast<uint32_t>(LIE::Debug::GetValidationLayers().size());
		createInfo.ppEnabledLayerNames = LIE::Debug::GetValidationLayers ().data ();
	}
	else{
		createInfo.enabledLayerCount = 0;
	}
}
}


