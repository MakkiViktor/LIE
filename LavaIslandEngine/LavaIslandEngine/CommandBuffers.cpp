#include "CommandBuffers.h"
#include "FrameBuffers.h"
#include "CommandPool.h"



namespace VK{

void CommandBuffers::Create (const CommandPool & commandPool,
							 const FrameBuffers & frameBuffers,
							 const DescriptorSets& descriptorSets,
							 const std::vector<GraphicsCommandDetails> graphicsDetails){
	commandBuffers.resize (frameBuffers.Size ());
	VkCommandBufferAllocateInfo allocInfo = {};
	FillAllocateInfo (allocInfo);
	allocInfo.commandPool = commandPool.GetCommandPool ();
	allocInfo.commandBufferCount = static_cast<U32>(commandBuffers.size ());

	if(vkAllocateCommandBuffers (commandPool.GetLogicalDevice (), &allocInfo, commandBuffers.data ()) != VK_SUCCESS){
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
		renderPassInfo.renderPass = frameBuffers.GetRenderPass ().GetRenderPass ();
		renderPassInfo.framebuffer = frameBuffers[i];

		vkCmdBeginRenderPass (commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
		FillCommands (commandBuffers[i], descriptorSets[i], graphicsDetails);
		if(vkEndCommandBuffer (commandBuffers[i]) != VK_SUCCESS){
			ERROR ("failed to record command buffer!");
		}

	}
	PRINT ("Command buffers set up");
}

void CommandBuffers::Create (const CommandPool & commandPool,
							 const FrameBuffers & frameBuffers,
							 const DescriptorSets& descriptorSets,
							 const std::vector<GraphicsData>& graphicsDatas){
	//If its slow start on new Thread the selecting section
	std::vector<GraphicsCommandDetails> details;
	for(U32 dataIndex = 0; dataIndex < graphicsDatas.size (); dataIndex++){
		std::vector<VkBuffer> vertexBuffers (graphicsDatas[dataIndex].vertexBuffers.size ());
		std::vector<VkDeviceSize> offsets (graphicsDatas[dataIndex].vertexBuffers.size ());
		SIZE offset = 0;
		for(U16 i = 0; i < vertexBuffers.size (); i++){
			vertexBuffers[i] = graphicsDatas[dataIndex].vertexBuffers[i].GetBuffer();
			offsets[i] = offset;
			offset += sizeof (vertexBuffers[i]);
		}
		details.push_back (GraphicsCommandDetails{graphicsDatas[dataIndex].pipeline,vertexBuffers, offsets , graphicsDatas[dataIndex].indexBuffer.GetBuffer(), graphicsDatas[dataIndex].indexBuffer.GetIndexCount ()});
	}

	Create (commandPool, frameBuffers, descriptorSets, details);
}

const VkCommandBuffer& CommandBuffers::operator[](U16 index) const{
	return commandBuffers[index];
}

SIZE CommandBuffers::Size () const{
	return commandBuffers.size();
}

const VkCommandBuffer * CommandBuffers::Data () const{
	return commandBuffers.data();
}

void CommandBuffers::FillCommands (const VkCommandBuffer& commandBuffer,
								   const VkDescriptorSet& descriptorSet,
								   const std::vector<GraphicsCommandDetails> graphicsDetails){
	for(const GraphicsCommandDetails& details : graphicsDetails){
		vkCmdBindPipeline (commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, details.pipeline.GetPipeline ());
		vkCmdBindVertexBuffers (commandBuffer, 0, static_cast<U32>(details.vertexBuffers.size ()), details.vertexBuffers.data (), details.offsets.data ());
		vkCmdBindIndexBuffer (commandBuffer, details.indexBuffer, 0, VK_INDEX_TYPE_UINT32);
		vkCmdBindDescriptorSets (commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, details.pipeline.GetPipelineLayout(), 0, 1, &descriptorSet, 0, nullptr);
		vkCmdDrawIndexed (commandBuffer, details.indexCount, 1, 0, 0, 0);
	}
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

