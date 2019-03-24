#ifndef LIE_VK_COMMAND_BUFFERS
#define LIE_VK_COMMAND_BUFFERS

#pragma once

#include "vulkan/vulkan.h"
#include <vector>
#include "Types.h"
#include "Buffer.h"

namespace VK{

class FrameBuffers;
class CommandPool;
class LogicalDevice;
class Pipeline;

class CommandBuffers{
private:
	std::vector<VkCommandBuffer> commandBuffers;

	virtual void FillCommands (const std::vector<Pipeline>& pipelines,
							   const VkCommandBuffer& commandBuffer,
							   const std::vector<VkBuffer> vertexBuffers,
							   const std::vector<VkDeviceSize> offsets);
	virtual void FillAllocateInfo (VkCommandBufferAllocateInfo& allocInfo);
	virtual void FillBeginInfo (VkCommandBufferBeginInfo & beginInfo);
	virtual void FillRenderPassInfo (VkRenderPassBeginInfo& renderPassInfo, VkClearValue& clearColor, const VkExtent2D& extent);
	void Create (const CommandPool& commandPool,
				 const std::vector<Pipeline>& pipelines,
				 const FrameBuffers& frameBuffers,
				 const std::vector<VkBuffer> vertexBuffers,
				 const std::vector<VkDeviceSize> offsets);
public:
	void Create (const CommandPool& commandPool, const std::vector<Pipeline>& pipelines,
				 const FrameBuffers& frameBuffers, const std::vector<Buffer> vertexBuffers);

	const VkCommandBuffer& operator [](U16 index) const;
	SIZE Size () const;
	const VkCommandBuffer* Data () const;
};

}

#endif