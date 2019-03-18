#ifndef LIE_VK_FENCE
#define LIE_VK_FENCE
#pragma once

#include "vulkan/vulkan.h"

namespace VK{

class LogicalDevice;

class Fence{
private:
	VkDevice device;
	VkFence fence;
	VkAllocationCallbacks* allocator;
public:
	void Create (const LogicalDevice& device, VkAllocationCallbacks* allocator = nullptr);
	void Destroy ();
	const VkFence& GetFence () const;

};

}

#endif 
