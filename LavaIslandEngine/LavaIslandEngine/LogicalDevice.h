#ifndef LIE_VK_LOGICAL_DEVICE
#define LIE_VK_LOGICAL_DEVICE

#pragma once

#include "PhysicalDevice.h"
#include "CopyInfo.h"

namespace VK{

struct QueueFamilyIndex;
class PhysicalDevice;
class Surface;

class LogicalDevice{
private:
	VkDevice logicalDevice = VK_NULL_HANDLE;
	PhysicalDevice physicalDevice;
	VkAllocationCallbacks* allocator;

	virtual void FillQueueCreateInfo (const QueueFamilyIndex& queueFamilyIndex,VkDeviceQueueCreateInfo& queueCreateInfo);
	virtual void FillDeviceCreateInfo (const std::vector<VkDeviceQueueCreateInfo>& queueCreateInfos,VkDeviceCreateInfo& createInfo);
public:
	void Create (const PhysicalDevice& physicalDevice, VkAllocationCallbacks* allocator = nullptr);
	void Destroy ();
	const VkDevice& GetLogicalDevice () const;
	const PhysicalDevice& GetPhysicalDevice () const;
};

}
#endif 

