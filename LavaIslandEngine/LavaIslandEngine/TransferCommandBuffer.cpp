#include "TransferCommandBuffer.h"
#include "CommandPool.h"

namespace VK{
void TransferCommandBuffer::FillAllocateInfo (VkCommandBufferAllocateInfo & allocInfo){
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = 1;
}

void TransferCommandBuffer::FillBeginInfo (VkCommandBufferBeginInfo & beginInfo){
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
}

void TransferCommandBuffer::FillCommands (VkCommandBuffer transferCommand, VkBuffer source, VkBuffer destination, SIZE size){
	VkBufferCopy copyRegion = {};
	copyRegion.srcOffset = 0; // Optional
	copyRegion.dstOffset = 0; // Optional
	copyRegion.size = size;
	vkCmdCopyBuffer (transferCommand, source, destination, 1, &copyRegion);
}

void TransferCommandBuffer::Create (const CommandPool& commandPool,
									VkBuffer source,
									VkBuffer destination,
									SIZE memorySize,
									VkAllocationCallbacks* allocator){
	device = commandPool.GetLogicalDevice ();

	VkCommandBufferAllocateInfo allocInfo = {};
	VkCommandBufferBeginInfo beginInfo = {};
	
	FillAllocateInfo (allocInfo);
	allocInfo.commandPool = commandPool.GetCommandPool();
	FillBeginInfo (beginInfo);
	vkAllocateCommandBuffers (commandPool.GetLogicalDevice(), &allocInfo, &transferCommand);
	vkBeginCommandBuffer (transferCommand, &beginInfo);
	FillCommands (transferCommand,source,destination, memorySize);
	vkEndCommandBuffer (transferCommand);

}

const VkCommandBuffer& TransferCommandBuffer::GetCommandBuffer (){
	return transferCommand;
}

VkCommandBuffer* TransferCommandBuffer::Data (){
	return &transferCommand;
}

}
