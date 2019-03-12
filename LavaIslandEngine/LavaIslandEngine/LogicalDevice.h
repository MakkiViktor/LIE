#ifndef LIE_VK_LOGICAL_DEVICE
#define LIE_VK_LOGICAL_DEVICE

#pragma once

#include "PhysicalDevice.h"

namespace VK{

struct QueueFamilyIndex;
class PhysicalDevice;
class Surface;

class LogicalDevice{
private:
	VkDevice logicalDevice = VK_NULL_HANDLE;
	const PhysicalDevice& physicalDevice;
	const VkAllocationCallbacks* allocator;
public:
	LogicalDevice (const PhysicalDevice& physicalDevice, const VkAllocationCallbacks* allocator = nullptr);
	~LogicalDevice ();
	const VkDevice& GetLogicalDevice () const;
	const PhysicalDevice& GetPhysicalDevice () const;
protected:
	virtual void FillQueueCreateInfo (const QueueFamilyIndex& queueFamilyIndex,VkDeviceQueueCreateInfo& queueCreateInfo);
	virtual void FillDeviceCreateInfo (const std::vector<VkDeviceQueueCreateInfo>& queueCreateInfos,VkDeviceCreateInfo& createInfo);

};

}
#endif 

