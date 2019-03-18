#ifndef LIE_VK_IMAGES
#define LIE_VK_IMAGES

#pragma once

#include "LogicalDevice.h"
#include "CopyInfo.h"

namespace VK{

class SwapChain;

class ImageViews{
private:
	std::vector<VkImageView> swapChainImageViews;
	VkExtent2D extent;
	VkDevice device;
	VkAllocationCallbacks* allocator;

	virtual void FillCreateInfo (VkImageViewCreateInfo & createInfo, const VkImage& image, const SwapChain& swapchain);
public:
	void Create (const SwapChain& swapChain, VkAllocationCallbacks* allocator = nullptr);
	void Destroy ();

	const std::vector<VkImageView>& GetImageViews () const;
	const VkExtent2D& GetExtent () const;
	VkDevice GetLogicalDevice () const;
};
}

#endif // !LIE_VK_IMAGES
