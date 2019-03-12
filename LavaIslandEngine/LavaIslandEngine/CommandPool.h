#ifndef LIE_VK_COMMAND_POOL
#define LIE_VK_COMMAND_POOL

#pragma once

#include "LogicalDevice.h"

namespace VK{
	
class CommandPool{
private:
	const LogicalDevice& device;
	const VkAllocationCallbacks* allocator;
	VkCommandPool commandPool;
public:
	CommandPool (const LogicalDevice& device, const VkAllocationCallbacks* allocator = nullptr);
	~CommandPool ();
	const VkCommandPool& GetCommandPool () const;
	const LogicalDevice& GetLogicalDevice () const;
protected:
	virtual void FillCreateInfo (VkCommandPoolCreateInfo poolInfo, const PhysicalDevice& physicalDevice);
};

}


#endif