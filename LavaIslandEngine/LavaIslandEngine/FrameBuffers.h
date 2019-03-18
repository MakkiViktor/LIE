#ifndef LIE_VK_FRAME_BUFFERS
#define LIE_VK_FRAME_BUFFERS 

#pragma once

#include "LogicalDevice.h"
#include "RenderPass.h"

namespace VK{

class ImageViews;

class FrameBuffers{
private:
	VkDevice device;
	VkExtent2D extent;
	std::vector<VkFramebuffer> frameBuffers;
	RenderPass renderPass;
	VkAllocationCallbacks* allocator;

	virtual void FillCreateInfo (VkFramebufferCreateInfo& framebufferInfo,
						 const std::vector<VkImageView>& attachments,
						 const RenderPass& renderPass, const VkExtent2D& extent);
	virtual void FillAttachments (std::vector<VkImageView>& attachments, const std::vector<VkImageView> swapChainImageViews, U16 index);
public:
	void Create (const ImageViews& imageViews, const RenderPass& renderPass, VkAllocationCallbacks* allocator = nullptr);
	void Destroy ();
	U16 Size () const;
	const RenderPass& GetRenderPass () const;
	const VkExtent2D& GetExtent () const;
	const VkFramebuffer& operator[](U16 index) const;
};

}

#endif 
