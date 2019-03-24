#pragma once
#ifndef LIE_TRANSFER_COMMAND_BUFFER
#define LIE_TRANSFER_COMMAND_BUFFER
#pragma once

#include "vulkan/vulkan.h"
#include "Types.h"

namespace VK{

class LogicalDevice;
class CommandPool;

class TransferCommandBuffer{
private:
	VkCommandBuffer transferCommand;
	VkDevice device;
	VkAllocationCallbacks* allocator;

	virtual void FillAllocateInfo (VkCommandBufferAllocateInfo& allocInfo);
	virtual void FillBeginInfo (VkCommandBufferBeginInfo & beginInfo);
	virtual void FillCommands (VkCommandBuffer transferCommand,
							   VkBuffer source,
							   VkBuffer destination,
							   SIZE size);
public:
	void Create (const CommandPool& commandPool,
				 VkBuffer source,
				 VkBuffer destination,
				 SIZE memorySize,
				 VkAllocationCallbacks* allocator = nullptr);
	const VkCommandBuffer& GetCommandBuffer ();
	VkCommandBuffer* Data ();
};
}

#endif // !LIE_TRANSFER_COMMAND_BUFFER
