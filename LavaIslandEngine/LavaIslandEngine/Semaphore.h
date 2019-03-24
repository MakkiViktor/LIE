#ifndef LIE_VK_SEMAPHORE
#define LIE_VK_SEMAPHORE
#pragma once

#include "LogicalDevice.h"

namespace VK{
class Semaphore{
private:
	VkSemaphore semaphore;
	VkDevice device;
	VkAllocationCallbacks* allocator;

	virtual void FillCreateInfo (VkSemaphoreCreateInfo& semaphoreInfo);
public:
	void Create (const VkDevice& device, VkAllocationCallbacks* allocator = nullptr);
	void Destroy ();
	VkSemaphore GetSemaphore () const;
};

}

#endif 
