#ifndef LIE_VK_DRAWER
#define LIE_VK_DRAWER
#pragma once

#include <vector>
#include "Semaphore.h"
#include "Fence.h"
#include "CommandBuffers.h"

namespace VK{

class SwapChain;
class Queue;
class Window;

struct DrawDetails{
	VkSwapchainKHR swapChain = VK_NULL_HANDLE;
	CommandBuffers commandBuffers;

	std::vector<Semaphore> renderFinishedSemaphores;
	std::vector<Semaphore> imageAvailableSemaphores;

	void Create (const VkDevice& device, U16 maxFramesInFlight);
	void Destroy ();
private:
	VkDevice device;
};

class Drawer{
private:
	struct FrameSyncObjects{
		std::vector<VkSemaphore> renderFinishedSemaphores;
		std::vector<VkSemaphore> imageAvailableSemaphores;
	};

	struct ImageObjects{
		std::vector<VkCommandBuffer> commandBuffers;
	};

	U16 maxFramesInFlight = 2;
	U16 currentFrame = 0;
	VkDevice device;
	std::vector<Fence> inFlightFences;
	std::vector<U32> imageIndexes;
	std::vector<VkSwapchainKHR> swapChains;
	std::vector<VkCommandBuffer> commanBuffers;
	std::vector<FrameSyncObjects> frameSyncObjects;
	std::vector<ImageObjects> imageObjects;
	std::vector<DrawDetails> drawDetails;

	void RefreshCommandBuffers ();

	static bool IsValidDetail (const DrawDetails& details);
	void CopyFrameSwapChains ();
	void CreateFrameObjects ();
	void CreateFences ();


	VkResult AquireImage (const VkSwapchainKHR& swapChai, U32& imageIndex, U16 swapChainIndex);
	VkResult AquireAllImages ();
	VkResult PresentQueue (const Queue& queue);
	void SubmitQueue (const Queue& queue);

public:
	void Create (const VkDevice& device, const std::vector<DrawDetails>& drawDetails);
	void Destroy ();

	VkResult Draw (const Queue& queue);
};
}


#endif 
