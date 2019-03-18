#include "Fence.h"
#include "LogicalDevice.h"

namespace VK{
void Fence::Create (const LogicalDevice & device, VkAllocationCallbacks* allocator ){
	this->device = device.GetLogicalDevice ();
	this->allocator = allocator;
	VkFenceCreateInfo fenceInfo = {};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
	if(vkCreateFence (this->device, &fenceInfo, allocator, &fence) != VK_SUCCESS){
		ERROR ("Failed to create Fence");
	}
}

void Fence::Destroy (){
	vkDestroyFence (device, fence, allocator);
}
const VkFence & Fence::GetFence () const{
	return fence;
}
}

