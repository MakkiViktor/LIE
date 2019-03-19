#ifndef LIE_VK_BASIC_RENDER_CORE
#define LIE_VK_BASIC_RENDER_CORE
#pragma once

#include "SwapChain.h"
#include "ImageViews.h"
#include "Pipeline.h"
#include "Shader.h"
#include "RenderPass.h"
#include "FrameBuffers.h"
#include "CommandPool.h"
#include "CommandBuffers.h"
#include "Surface.h"
#include "Semaphore.h"
#include "Fence.h"

namespace VK{

class Queue;
class Window;

class BasicRenderCore{
private:
	U16 maxFramesInFlight = 2;
	U16 currentFrame = 0;
	bool framebufferResized = false;

	LogicalDevice logicalDevice;
	Surface surface;
	SwapChain swapChain;
	ImageViews imageviews;
	RenderPass renderPass;
	CommandPool commandPool;
	FrameBuffers frameBuffers;
	CommandBuffers commandBuffers;
	std::vector<Semaphore> renderFinishedSemaphores;
	std::vector<Semaphore> imageAvailableSemaphores;
	std::vector<Fence> inFlightFences;

	std::vector<Pipeline> pipelines;

	virtual SwapChain CreateSwapChain (const LogicalDevice& logicalDevice, const Surface& surface);
	virtual ImageViews CreateImageView (const SwapChain& swapChain);
	virtual RenderPass CreateRenderPass (const SwapChain& swapChain);
	virtual FrameBuffers CreateFrameBuffers (const ImageViews& imageViews, const RenderPass& renderPass);
	virtual void CreatePipelines (std::vector<Pipeline>& pipelines ,const SwapChain& swapChain, const RenderPass& renderPass);
	virtual CommandPool CreateCommandPool (const LogicalDevice& logicalDevice);
	virtual CommandBuffers CreateCommandBuffers (const CommandPool& commandPool, const std::vector<Pipeline>& pipelines, const FrameBuffers& frameBuffers);

	void Recreate (Window& window);
public:
	void Create (const LogicalDevice& logicalDevice, const Surface& surface);
	void Destroy ();
	
	void Resize ();
	virtual void Draw (const Queue& queue, Window& window);

};
}

#endif 
