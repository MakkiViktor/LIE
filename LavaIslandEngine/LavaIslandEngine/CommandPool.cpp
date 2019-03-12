#include "CommandPool.h"

namespace VK{

CommandPool::CommandPool (const LogicalDevice & device, const VkAllocationCallbacks* allocator):
device(device),
allocator(allocator){
	VkCommandPoolCreateInfo poolInfo = {};
	FillCreateInfo (poolInfo, device.GetPhysicalDevice ());
	if(vkCreateCommandPool (device.GetLogicalDevice(), &poolInfo, nullptr, &commandPool) != VK_SUCCESS){
		ERROR ("failed to create command pool!");
	}
	PRINT ("Command pool created");
}

CommandPool::~CommandPool (){
	vkDestroyCommandPool (device.GetLogicalDevice(), commandPool, allocator);
	PRINT ("Command pool destroyed");
}

const VkCommandPool & CommandPool::GetCommandPool () const{
	return commandPool;
}

const LogicalDevice & CommandPool::GetLogicalDevice () const{
	return device;
}

void CommandPool::FillCreateInfo (VkCommandPoolCreateInfo poolInfo, const PhysicalDevice & physicalDevice){
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.queueFamilyIndex = physicalDevice.GetQueueFamilyIndexByFlag(VK_QUEUE_GRAPHICS_BIT).queueFamily.value();
	poolInfo.flags = 0; // Optional
}

}
