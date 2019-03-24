#include "TransferPool.h"
#include "TransferCommandBuffer.h"

namespace VK{
void VK::TransferPool::FillCreateInfo (VkCommandPoolCreateInfo poolInfo, const PhysicalDevice & physicalDevice){
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.queueFamilyIndex = physicalDevice.GetQueueFamilyIndexByFlag (VK_QUEUE_GRAPHICS_BIT).queueFamily.value ();
	poolInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
}
void TransferPool::Free (TransferCommandBuffer& commandBuffers){
	vkFreeCommandBuffers (GetLogicalDevice(), GetCommandPool(), 1, commandBuffers.Data());
}
}

