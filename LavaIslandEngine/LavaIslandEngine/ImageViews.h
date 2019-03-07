#ifndef LIE_VK_IMAGES
#define LIE_VK_IMAGES

#pragma once

#include "vulkan/vulkan.h"
#include <vector>
#include "LogicalDevice.h"

namespace VK{

class SwapChain;

class ImageViewsDetails{
private:
	VkImageViewType viewType;
	VkComponentMapping mapping;
	VkImageSubresourceRange subresourceRange;
public:
	ImageViewsDetails();

	void SetViewType (VkImageViewType viewType);
	void SetComponentMapping (VkComponentMapping componentMapping);
	void SetSubresourceRange (VkImageSubresourceRange subresourceRange);

	VkImageViewType GetViewType () const;
	VkComponentMapping GetComponentMapping () const;
	VkImageSubresourceRange GetSubresourceRange () const;
};

class ImageViews{
private:
	std::vector<VkImageView> swapChainImageViews;
	const LogicalDevice& device;
	const VkAllocationCallbacks* allocator;

	void FillCreateInfo (VkImageViewCreateInfo & createInfo, const VkImage& image, const SwapChain& swapchain, const ImageViewsDetails & details);
public:
	ImageViews (const LogicalDevice& device, const SwapChain& swapChain, const ImageViewsDetails& details, const VkAllocationCallbacks* allocator = nullptr);
	~ImageViews ();
};
}

#endif // !LIE_VK_IMAGES
