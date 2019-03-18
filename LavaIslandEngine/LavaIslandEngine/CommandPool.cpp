#include "CommandPool.h"

namespace VK{


void CommandPool::Create (const LogicalDevice & device, VkAllocationCallbacks* allocator){
	this->device = device.GetLogicalDevice();
	allocator = allocator;
	VkCommandPoolCreateInfo poolInfo = {};
	FillCreateInfo (poolInfo, device.GetPhysicalDevice ());
	if(vkCreateCommandPool (device.GetLogicalDevice (), &poolInfo, nullptr, &commandPool) != VK_SUCCESS){
		ERROR ("failed to create command pool!");
	}
	PRINT ("Command pool created");
}

void CommandPool::Destroy (){
	if(commandPool != VK_NULL_HANDLE){
		vkDestroyCommandPool (device, commandPool, allocator);
		PRINT ("Command pool destroyed");
		commandPool = VK_NULL_HANDLE;
	}
}

const VkCommandPool & CommandPool::GetCommandPool () const{
	return commandPool;
}

VkDevice CommandPool::GetLogicalDevice () const{
	return device;
}

void CommandPool::FillCreateInfo (VkCommandPoolCreateInfo poolInfo, const PhysicalDevice & physicalDevice){
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.queueFamilyIndex = physicalDevice.GetQueueFamilyIndexByFlag(VK_QUEUE_GRAPHICS_BIT).queueFamily.value();
	poolInfo.flags = 0; // Optional
}

}
