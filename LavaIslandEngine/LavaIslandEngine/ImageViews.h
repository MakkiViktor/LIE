#ifndef LIE_VK_IMAGES
#define LIE_VK_IMAGES

#pragma once

#include "vulkan/vulkan.h"
#include <vector>
#include "LogicalDevice.h"

namespace VK{

class SwapChain;

class ImageViews{
private:
	std::vector<VkImageView> swapChainImageViews;
	const VkExtent2D extent;
	const LogicalDevice& device;
	const VkAllocationCallbacks* allocator;
public:
	ImageViews (const SwapChain& swapChain, const VkAllocationCallbacks* allocator = nullptr);
	~ImageViews ();
	const std::vector<VkImageView>& GetImageViews () const;
	const VkExtent2D& GetExtent () const;
	const LogicalDevice& GetLogicalDevice () const;
protected:
	virtual void FillCreateInfo (VkImageViewCreateInfo & createInfo, const VkImage& image, const SwapChain& swapchain);
};
}

#endif // !LIE_VK_IMAGES
