#include "ImageViews.h"

#include "SwapChain.h"

namespace VK{
//ImageViews
ImageViews::ImageViews (const SwapChain & swapChain, const VkAllocationCallbacks* allocator):
device(swapChain.GetLogicalDevice()),
allocator(allocator),
extent(swapChain.GetExtent())
{
	std::vector<VkImage> images (swapChain.GetImages ());
	swapChainImageViews.resize (images.size ());

	for(size_t i = 0; i < images.size (); i++){
		VkImageViewCreateInfo createInfo = {};
		FillCreateInfo (createInfo, images[i], swapChain);

		if(vkCreateImageView (device.GetLogicalDevice(), &createInfo, allocator, &swapChainImageViews[i]) != VK_SUCCESS){
			ERROR ("failed to create image views!");
		}
		PRINT (std::string("Image View Created"));
	}
}

ImageViews::~ImageViews (){
	for(auto imageView : swapChainImageViews){
		vkDestroyImageView (device.GetLogicalDevice(), imageView, allocator);
		PRINT (std::string ("Image View Destroyed"));
	}
}

const std::vector<VkImageView>& ImageViews::GetImageViews () const{
	return swapChainImageViews;
}

const VkExtent2D & ImageViews::GetExtent () const{
	return extent;
}

const LogicalDevice & ImageViews::GetLogicalDevice () const{
	return device;
}

void ImageViews::FillCreateInfo (VkImageViewCreateInfo& createInfo,const VkImage& image,const SwapChain& swapChain){
	VkImageViewType viewType (VK_IMAGE_VIEW_TYPE_2D);
	VkComponentMapping mapping;
	VkImageSubresourceRange subresourceRange;

	mapping.r = VK_COMPONENT_SWIZZLE_IDENTITY;
	mapping.g = VK_COMPONENT_SWIZZLE_IDENTITY;
	mapping.b = VK_COMPONENT_SWIZZLE_IDENTITY;
	mapping.a = VK_COMPONENT_SWIZZLE_IDENTITY;

	subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	subresourceRange.baseMipLevel = 0;
	subresourceRange.levelCount = 1;
	subresourceRange.baseArrayLayer = 0;
	subresourceRange.layerCount = 1;

	createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	createInfo.image = image;
	createInfo.viewType = viewType;
	createInfo.format = swapChain.GetFormat ();
	createInfo.components = mapping;
	createInfo.subresourceRange = subresourceRange;
}
}