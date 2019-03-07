#ifndef LIE_VK_QUEUE
#define LIE_VK_QUEUE

#pragma once

#include "vulkan/vulkan.h"

namespace VK{

class LogicalDevice;
struct QueueFamilyIndex;

class Queue{
private:
	VkQueue queue;
public:
	Queue (const LogicalDevice& logicalDevice, const QueueFamilyIndex& queueFamilyIndex);
	~Queue ();
};

}
#endif