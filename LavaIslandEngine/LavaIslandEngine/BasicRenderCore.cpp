#include "BasicRenderCore.h"

#include "Queue.h"
#include "Window.h"
#include "SharedResources.h"
#include "VertexBuffer.h"
#include "StagingBuffer.h"
#include "BufferTransferer.h"

namespace VK{

void BasicRenderCore::Create (const LogicalDevice& logicalDevice, const Surface& surface){
	this->logicalDevice = logicalDevice;
	this->surface = surface;
	swapChain = CreateSwapChain (logicalDevice, surface);
	imageviews = CreateImageView (swapChain);
	renderPass = CreateRenderPass (swapChain);
	frameBuffers = CreateFrameBuffers (imageviews, renderPass);
	CreatePipelines (pipelines, swapChain, renderPass);
	commandPool = CreateCommandPool (logicalDevice);
	CreateBuffers (logicalDevice, buffers);
	commandBuffers = CreateCommandBuffers (commandPool, pipelines, frameBuffers, buffers);


}

void BasicRenderCore::Destroy (){
	for(Buffer& buffer : buffers)
		buffer.Destroy ();
	commandPool.Destroy ();
	frameBuffers.Destroy ();
	for(Pipeline& pipeline : pipelines)
		pipeline.Destroy ();
	renderPass.Destroy ();
	imageviews.Destroy ();
	swapChain.Destroy ();
}

VkSwapchainKHR BasicRenderCore::GetSwapChain (){
	return swapChain.GetSwapchain();
}

const CommandBuffers & BasicRenderCore::GetCommandBuffers (){
	return commandBuffers;
}

void BasicRenderCore::Recreate (Window& window){
	window.Refresh();
	sharedWindow = window;
	frameBuffers.Destroy ();
	commandPool.Free (commandBuffers);
	for(Pipeline& pipeline : pipelines)
		pipeline.Destroy ();
	pipelines.clear ();
	renderPass.Destroy ();
	imageviews.Destroy ();
	swapChain.Destroy ();

	vkDeviceWaitIdle (logicalDevice.GetLogicalDevice ());

	swapChain = CreateSwapChain (logicalDevice, surface);
	imageviews = CreateImageView (swapChain);
	renderPass = CreateRenderPass (swapChain);
	CreatePipelines (pipelines, swapChain, renderPass);
	frameBuffers = CreateFrameBuffers (imageviews, renderPass);
	commandBuffers = CreateCommandBuffers (commandPool, pipelines, frameBuffers, buffers);

}

void BasicRenderCore::Draw (const Queue& queue, Window& window){

}

SwapChain BasicRenderCore::CreateSwapChain (const LogicalDevice & logicalDevice, const Surface & surface){
	SwapChain swapChain;
	swapChain.Create (logicalDevice, surface);
	return swapChain;
}

ImageViews BasicRenderCore::CreateImageView (const SwapChain & swapChain){
	ImageViews imagaviews;
	imagaviews.Create (swapChain);
	return imagaviews;
}

RenderPass BasicRenderCore::CreateRenderPass (const SwapChain & swapChain){
	RenderPass renderPass;
	renderPass.Create (swapChain);
	return renderPass;
}

void BasicRenderCore::CreatePipelines (std::vector<Pipeline>& pipelines, const SwapChain & swapChain, const RenderPass & renderPass){
	Pipeline pipeline;
	pipeline.Create <Vertex> ({VK::ShaderDetails ("Shaders/vert.spv",VK_SHADER_STAGE_VERTEX_BIT),
					  VK::ShaderDetails ("Shaders/frag.spv",VK_SHADER_STAGE_FRAGMENT_BIT)},
					 swapChain, renderPass);
	pipelines.push_back (pipeline);
}

FrameBuffers BasicRenderCore::CreateFrameBuffers (const ImageViews & imageViews, const RenderPass & renderPass){
	FrameBuffers frameBuffers;
	frameBuffers.Create (imageViews, renderPass);
	return frameBuffers;
}

CommandPool BasicRenderCore::CreateCommandPool (const LogicalDevice & logicalDevice){
	CommandPool commandPool;
	commandPool.Create (logicalDevice);
	return commandPool;
}

void BasicRenderCore::CreateBuffers (const LogicalDevice & logicalDevice, std::vector<Buffer>& vertexBuffers){
	std::vector<Vertex> vertices = {
	{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
	{{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
	{{-0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}}};
	
	VertexBuffer<Vertex> vertexBuffer;
	vertexBuffer.Create(logicalDevice, vertices);

	StagingBuffer stagingBuffer;
	stagingBuffer.CreateBuffer (logicalDevice, vertexBuffer.GetSize ());
	BufferTransferer::MapMemory (stagingBuffer, vertices.data ());
	BufferTransferer::TransferBufferData (stagingBuffer, vertexBuffer);
	stagingBuffer.Destroy ();
	vertexBuffers.push_back (vertexBuffer);
}

CommandBuffers BasicRenderCore::CreateCommandBuffers (const CommandPool& commandPool, const std::vector<Pipeline>& pipelines, const FrameBuffers & frameBuffers, const std::vector<Buffer>& vertexBuffers){
	CommandBuffers commandBuffers;
	commandBuffers.Create (commandPool, pipelines, frameBuffers, vertexBuffers);
	return commandBuffers;
}

}