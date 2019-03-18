#ifndef LIE_VK_QUEUE
#define LIE_VK_QUEUE

#pragma once

#include "vulkan/vulkan.h"

namespace VK{

class LogicalDevice;
struct QueueFamilyIndex;

class Queue{
private:
	VkQueue queue = VK_NULL_HANDLE;
public:
	void Create (const LogicalDevice& logicalDevice, const QueueFamilyIndex& queueFamilyIndex);

	const VkQueue& GetQueue () const;
};

}
#endif