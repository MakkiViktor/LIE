#ifndef LIE_VK_FRAME_BUFFERS
#define LIE_VK_FRAME_BUFFERS 

#pragma once

#include "LogicalDevice.h"
#include "RenderPass.h"

namespace VK{

class ImageViews;

class FrameBuffers{
private:
	const LogicalDevice& device;
	const VkExtent2D extent;
	std::vector<VkFramebuffer> frameBuffers;
	const std::vector<VkImageView>& swapChainImageViews;
	const RenderPass& renderPass;
	const VkAllocationCallbacks* allocator;
public:
	FrameBuffers (const ImageViews& imageViews, const RenderPass& renderPass, const VkAllocationCallbacks* allocator = nullptr);
	~FrameBuffers ();
	U16 Size () const;
	const RenderPass& GetRenderPass () const;
	const VkExtent2D& GetExtent () const;
	const VkFramebuffer& operator[](U16 index) const;
protected:
	virtual void FillCreateInfo (VkFramebufferCreateInfo& framebufferInfo,
						 const std::vector<VkImageView>& attachments,
						 const RenderPass& renderPass, const VkExtent2D& extent);
	virtual void FillAttachments (std::vector<VkImageView>& attachments, U16 index);
};

}

#endif 
