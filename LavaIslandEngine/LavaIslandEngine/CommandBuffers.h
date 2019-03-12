#ifndef LIE_VK_COMMAND_BUFFERS
#define LIE_VK_COMMAND_BUFFERS

#pragma once

#include "vulkan/vulkan.h"
#include <vector>

namespace VK{

class FrameBuffers;
class CommandPool;
class LogicalDevice;
class Pipeline;

class CommandBuffers{
private:
	std::vector<VkCommandBuffer> commandBuffers;
public:
	CommandBuffers (const CommandPool& commandPool, const Pipeline & pipeline,
					const FrameBuffers& frameBuffers);
protected:
	virtual void FillCommands (const Pipeline& pipeline, const VkCommandBuffer& commandBuffer);
	virtual void FillAllocateInfo (VkCommandBufferAllocateInfo& allocInfo);
	virtual void FillBeginInfo (VkCommandBufferBeginInfo & beginInfo);
	virtual void FillRenderPassInfo (VkRenderPassBeginInfo& renderPassInfo, VkClearValue& clearColor, const VkExtent2D& extent);
};

}

#endif