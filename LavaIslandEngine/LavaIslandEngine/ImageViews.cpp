#include "ImageViews.h"

#include "SwapChain.h"

namespace VK{
//ImageViews
ImageViews::ImageViews (const LogicalDevice& device,const SwapChain & swapChain, const ImageViewsDetails& details, const VkAllocationCallbacks* allocator):
device(device),
allocator(allocator)
{
	std::vector<VkImage> images (swapChain.GetImages ());
	swapChainImageViews.resize (images.size ());

	for(size_t i = 0; i < images.size (); i++){
		VkImageViewCreateInfo createInfo = {};
		FillCreateInfo (createInfo, images[i], swapChain, details);

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

void ImageViews::FillCreateInfo (VkImageViewCreateInfo& createInfo,const VkImage& image,const SwapChain& swapChain,const ImageViewsDetails & details){
	createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	createInfo.image = image;
	createInfo.viewType = details.GetViewType ();
	createInfo.format = swapChain.GetFormat ();
	createInfo.components = details.GetComponentMapping ();
	createInfo.subresourceRange = details.GetSubresourceRange ();
}

//ImageViewsDetails
ImageViewsDetails::ImageViewsDetails ():
viewType(VK_IMAGE_VIEW_TYPE_2D){
	mapping.r = VK_COMPONENT_SWIZZLE_IDENTITY;
	mapping.g = VK_COMPONENT_SWIZZLE_IDENTITY;
	mapping.b = VK_COMPONENT_SWIZZLE_IDENTITY;
	mapping.a = VK_COMPONENT_SWIZZLE_IDENTITY;

	subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	subresourceRange.baseMipLevel = 0;
	subresourceRange.levelCount = 1;
	subresourceRange.baseArrayLayer = 0;
	subresourceRange.layerCount = 1;
}

void ImageViewsDetails::SetViewType (VkImageViewType viewType){
	this->viewType = viewType;
}
void ImageViewsDetails::SetComponentMapping (VkComponentMapping componentMapping){
	mapping = componentMapping;
}
void ImageViewsDetails::SetSubresourceRange (VkImageSubresourceRange subresourceRange){
	this->subresourceRange = subresourceRange;
}

VkImageViewType ImageViewsDetails::GetViewType () const{
	return viewType;
}
VkComponentMapping ImageViewsDetails::GetComponentMapping () const{
	return mapping;
}
VkImageSubresourceRange ImageViewsDetails::GetSubresourceRange () const{
	return subresourceRange;
}
}