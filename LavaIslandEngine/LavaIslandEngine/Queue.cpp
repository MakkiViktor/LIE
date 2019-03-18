#include "Queue.h"
#include "LogicalDevice.h"
#include "PhysicalDevice.h"

namespace VK{

void Queue::Create (const LogicalDevice & logicalDevice, const QueueFamilyIndex & queueFamilyIndex){
	vkGetDeviceQueue (logicalDevice.GetLogicalDevice (), queueFamilyIndex.queueFamily.value (), 0, &queue);
}

const VkQueue & Queue::GetQueue () const{
	return queue;
}

}