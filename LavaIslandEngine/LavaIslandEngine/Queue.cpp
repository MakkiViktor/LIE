#include "Queue.h"
#include "LogicalDevice.h"
#include "PhysicalDevice.h"

namespace VK{

Queue::Queue (const LogicalDevice& logicalDevice, const QueueFamilyIndex& queueFamilyIndex){
	vkGetDeviceQueue (logicalDevice.GetLogicalDevice(), queueFamilyIndex.queueFamily.value (), 0, &queue);
}
Queue::~Queue (){}
}
