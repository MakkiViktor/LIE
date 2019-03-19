#include "BasicRenderCore.h"

#include "Queue.h"
#include "Window.h"
#include "SharedResources.h"

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
	commandBuffers = CreateCommandBuffers (commandPool, pipelines, frameBuffers);

	for(U16 i = 0; i < maxFramesInFlight; i++){
		Semaphore renderFinishedSemaphore;
		Semaphore imageAvailableSemaphore;
		Fence	fence;
		renderFinishedSemaphore.Create (logicalDevice);
		imageAvailableSemaphore.Create (logicalDevice);
		fence.Create (logicalDevice);
		inFlightFences.push_back (fence);
		renderFinishedSemaphores.push_back (renderFinishedSemaphore);
		imageAvailableSemaphores.push_back (imageAvailableSemaphore);
	}
}

void BasicRenderCore::Destroy (){
	for(Semaphore& semaphore : renderFinishedSemaphores)
		semaphore.Destroy ();
	for(Semaphore& semaphore : imageAvailableSemaphores)
		semaphore.Destroy ();
	for(Fence& fence: inFlightFences)
		fence.Destroy ();
	commandPool.Destroy ();
	frameBuffers.Destroy ();
	for(Pipeline& pipeline : pipelines)
		pipeline.Destroy ();
	renderPass.Destroy ();
	imageviews.Destroy ();
	swapChain.Destroy ();
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
	commandBuffers = CreateCommandBuffers (commandPool, pipelines, frameBuffers);

}

void BasicRenderCore::Resize (){
	framebufferResized = true;
}

void BasicRenderCore::Draw (const Queue& queue, Window& window){
	U32 imageIndex;
	vkWaitForFences (logicalDevice.GetLogicalDevice(),
					 1, &inFlightFences[currentFrame].GetFence(),
					 VK_TRUE, std::numeric_limits<uint64_t>::max ());

	

	VkResult result = vkAcquireNextImageKHR (logicalDevice.GetLogicalDevice (),
						   swapChain.GetSwapchain(),
						   std::numeric_limits<uint64_t>::max (),
						   imageAvailableSemaphores[currentFrame].GetSemaphore (),
						   VK_NULL_HANDLE,
						   &imageIndex);

	if(result == VK_ERROR_OUT_OF_DATE_KHR){
		Recreate (window);
		return;
	}
	else if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR){
		ERROR ("failed to acquire swap chain image!");
	}

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame].GetSemaphore()};
	VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffers[imageIndex];

	VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame].GetSemaphore()};
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	vkResetFences (logicalDevice.GetLogicalDevice (),
				   1, &inFlightFences[currentFrame].GetFence());

	if(vkQueueSubmit (queue.GetQueue(), 1, &submitInfo, inFlightFences[currentFrame].GetFence()) != VK_SUCCESS){
		ERROR ("failed to submit draw command buffer!");
	}

	VkSwapchainKHR swapChains[] = {swapChain.GetSwapchain()};
	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;

	result = vkQueuePresentKHR (queue.GetQueue(), &presentInfo);
	if(result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized){
		framebufferResized = false;
		Recreate (window);
	}
	else if(result != VK_SUCCESS){
		throw std::runtime_error ("failed to present swap chain image!");
	}

	currentFrame = (currentFrame + 1) % maxFramesInFlight;
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
	pipeline.Create ({VK::ShaderDetails ("Shaders/vert.spv",VK_SHADER_STAGE_VERTEX_BIT),
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

CommandBuffers BasicRenderCore::CreateCommandBuffers (const CommandPool& commandPool, const std::vector<Pipeline>& pipelines, const FrameBuffers & frameBuffers){
	CommandBuffers commandBuffers;
	commandBuffers.Create (commandPool, pipelines, frameBuffers);
	return commandBuffers;
}

}