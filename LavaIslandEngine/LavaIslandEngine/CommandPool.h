#ifndef LIE_VK_COMMAND_POOL
#define LIE_VK_COMMAND_POOL

#pragma once

#include "LogicalDevice.h"

namespace VK{
	
class CommandPool{
private:
	VkDevice device;
	VkAllocationCallbacks* allocator;
	VkCommandPool commandPool = VK_NULL_HANDLE;
	virtual void FillCreateInfo (VkCommandPoolCreateInfo poolInfo, const PhysicalDevice& physicalDevice);
public:
	void Create (const LogicalDevice& device, VkAllocationCallbacks* allocator = nullptr);
	void Destroy ();
	const VkCommandPool& GetCommandPool () const;
	VkDevice GetLogicalDevice () const;

};

}


#endif