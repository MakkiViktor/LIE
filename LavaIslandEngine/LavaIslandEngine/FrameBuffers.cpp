#include "FrameBuffers.h"

#include "ImageViews.h"

namespace VK{

void FrameBuffers::Create (const ImageViews& imageViews, const RenderPass& renderPass, VkAllocationCallbacks* allocator){
	device = imageViews.GetLogicalDevice ();
	extent = imageViews.GetExtent ();
	this->renderPass = renderPass;
	std::vector<VkImageView> swapChainImageViews = imageViews.GetImageViews ();

	frameBuffers.resize (swapChainImageViews.size ());
	std::vector<VkImageView> attachments;

	for(U16 i = 0; i < swapChainImageViews.size (); i++){
		attachments.clear ();
		VkFramebufferCreateInfo framebufferInfo = {};
		FillAttachments (attachments, swapChainImageViews, i);
		FillCreateInfo (framebufferInfo, attachments, renderPass, extent);
		if(vkCreateFramebuffer (device, &framebufferInfo, allocator, &frameBuffers[i]) != VK_SUCCESS){
			ERROR ("failed to create framebuffer!");
		}
	}
	PRINT ("Frame buffers created");
}

void FrameBuffers::Destroy (){
	for(auto framebuffer : frameBuffers){
		if(framebuffer != VK_NULL_HANDLE){
			vkDestroyFramebuffer (device, framebuffer, allocator);
			framebuffer = VK_NULL_HANDLE;
			PRINT ("Frame buffer destroyed");
		}
	}
}

U16 FrameBuffers::Size () const{
	return static_cast<U16>(frameBuffers.size());
}

const RenderPass & FrameBuffers::GetRenderPass () const{
	return renderPass;
}

const VkExtent2D & FrameBuffers::GetExtent () const{
	return extent;
}

const VkFramebuffer& FrameBuffers::operator[](U16 index) const{
	return frameBuffers[index];
}

void FrameBuffers::FillCreateInfo (VkFramebufferCreateInfo & framebufferInfo,
								   const std::vector<VkImageView>& attachments,
								   const RenderPass & renderPass, const VkExtent2D & extent){
	framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	framebufferInfo.renderPass = renderPass.GetRenderPass ();
	framebufferInfo.attachmentCount = static_cast<U32>(attachments.size());
	framebufferInfo.pAttachments = attachments.data();
	framebufferInfo.width = extent.width;
	framebufferInfo.height = extent.height;
	framebufferInfo.layers = 1;
}

void FrameBuffers::FillAttachments (std::vector<VkImageView>& attachments, const std::vector<VkImageView> swapChainImageViews, U16 index){
	attachments.push_back (swapChainImageViews[index]);
}

}
