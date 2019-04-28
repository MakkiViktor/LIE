#ifndef LIE_VK_COMMAND_BUFFERS
#define LIE_VK_COMMAND_BUFFERS

#pragma once

#include "vulkan/vulkan.h"
#include <vector>
#include "Types.h"
#include "Pipeline.h"
#include "IndexBuffer.h"
#include "DescriptorSets.h"

namespace VK{

class FrameBuffers;
class Buffer;
class CommandPool;
class LogicalDevice;


struct GraphicsData{
	Pipeline pipeline;
	std::vector<Buffer> vertexBuffers;
	IndexBuffer indexBuffer;
};

class CommandBuffers{
private:
	struct GraphicsCommandDetails{
		Pipeline pipeline;
		std::vector<VkBuffer> vertexBuffers;
		std::vector<VkDeviceSize> offsets;
		VkBuffer indexBuffer;
		U32 indexCount;
	};

	std::vector<VkCommandBuffer> commandBuffers;

	virtual void FillCommands (const VkCommandBuffer& commandBuffer,
							   const VkDescriptorSet& descriptorSet,
							   const std::vector<GraphicsCommandDetails> graphicsDetails);

	virtual void FillAllocateInfo (VkCommandBufferAllocateInfo& allocInfo);
	virtual void FillBeginInfo (VkCommandBufferBeginInfo & beginInfo);
	virtual void FillRenderPassInfo (VkRenderPassBeginInfo& renderPassInfo, VkClearValue& clearColor, const VkExtent2D& extent);
	void Create (const CommandPool& commandPool,
				 const FrameBuffers& frameBuffers,
				 const DescriptorSets& descriptorSets,
				 const std::vector<GraphicsCommandDetails> graphicsDetails);
public:
	void Create (const CommandPool& commandPool,
				 const FrameBuffers& frameBuffers,
				 const DescriptorSets& descriptorSets,
				 const std::vector<GraphicsData>& graphicsDatas);

	const VkCommandBuffer& operator [](U16 index) const;
	SIZE Size () const;
	const VkCommandBuffer* Data () const;
};

}

#endif