#ifndef LIE_VK_COMMAND_BUFFERS
#define LIE_VK_COMMAND_BUFFERS

#pragma once

#include "vulkan/vulkan.h"
#include <vector>
#include "Types.h"

namespace VK{

class FrameBuffers;
class CommandPool;
class LogicalDevice;
class Pipeline;

class CommandBuffers{
private:
	std::vector<VkCommandBuffer> commandBuffers;

	virtual void FillCommands (const std::vector<Pipeline>& pipelines, const VkCommandBuffer& commandBuffer);
	virtual void FillAllocateInfo (VkCommandBufferAllocateInfo& allocInfo);
	virtual void FillBeginInfo (VkCommandBufferBeginInfo & beginInfo);
	virtual void FillRenderPassInfo (VkRenderPassBeginInfo& renderPassInfo, VkClearValue& clearColor, const VkExtent2D& extent);
public:
	void Create (const CommandPool& commandPool, const std::vector<Pipeline>& pipelines,
				 const FrameBuffers& frameBuffers);
	const VkCommandBuffer& operator [](U16 index) const;
};

}

#endif