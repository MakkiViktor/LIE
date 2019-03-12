#include "CommandBuffers.h"
#include "FrameBuffers.h"
#include "CommandPool.h"
#include "Pipeline.h"

namespace VK{

CommandBuffers::CommandBuffers (const CommandPool& commandPool, const Pipeline & pipeline, const FrameBuffers& frameBuffers){
	commandBuffers.resize (frameBuffers.Size());
	VkCommandBufferAllocateInfo allocInfo = {};
	FillAllocateInfo (allocInfo);
	allocInfo.commandPool = commandPool.GetCommandPool();
	allocInfo.commandBufferCount = static_cast<U32>(commandBuffers.size ());

	if(vkAllocateCommandBuffers (commandPool.GetLogicalDevice().GetLogicalDevice(), &allocInfo, commandBuffers.data ()) != VK_SUCCESS){
		ERROR ("failed to allocate command buffers!");
	}

	for(U16 i = 0; i < commandBuffers.size (); i++){
		
		VkCommandBufferBeginInfo beginInfo = {};
		FillBeginInfo (beginInfo);
		if(vkBeginCommandBuffer (commandBuffers[i], &beginInfo) != VK_SUCCESS){
			ERROR ("failed to begin recording command buffer!");
		}

		VkRenderPassBeginInfo renderPassInfo = {};
		VkClearValue clearColor = {0.0f, 0.0f, 0.0f, 1.0f};
		FillRenderPassInfo (renderPassInfo, clearColor, frameBuffers.GetExtent ());
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = frameBuffers.GetRenderPass().GetRenderPass();
		renderPassInfo.framebuffer = frameBuffers[i];

		vkCmdBeginRenderPass (commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
		FillCommands (pipeline, commandBuffers[i]);
		if(vkEndCommandBuffer (commandBuffers[i]) != VK_SUCCESS){
			ERROR ("failed to record command buffer!");
		}
		
	}
	PRINT ("Command buffers set up");

}

void CommandBuffers::FillCommands (const Pipeline& pipeline, const VkCommandBuffer& commandBuffer){
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.GetPipeline());
		vkCmdDraw (commandBuffer, 3, 1, 0, 0);
}

void CommandBuffers::FillAllocateInfo (VkCommandBufferAllocateInfo& allocInfo){
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
}

void CommandBuffers::FillBeginInfo (VkCommandBufferBeginInfo& beginInfo){
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
}

void CommandBuffers::FillRenderPassInfo (VkRenderPassBeginInfo& renderPassInfo, VkClearValue& clearColor, const VkExtent2D& extent){
	renderPassInfo.renderArea.offset = {0, 0};
	renderPassInfo.renderArea.extent = extent;
	renderPassInfo.clearValueCount = 1;
	renderPassInfo.pClearValues = &clearColor;
}

}
