#include "FrameBuffers.h"

#include "ImageViews.h"

namespace VK{

FrameBuffers::FrameBuffers (const ImageViews& imageViews, const RenderPass& renderPass, const VkAllocationCallbacks* allocator):
device(imageViews.GetLogicalDevice()),
swapChainImageViews (imageViews.GetImageViews ()),
renderPass(renderPass),
extent(imageViews.GetExtent ()),
allocator(allocator){
	frameBuffers.resize (swapChainImageViews.size ());
	std::vector<VkImageView> attachments;
	
	for(U16 i = 0; i < swapChainImageViews.size (); i++){
		attachments.clear ();
		VkFramebufferCreateInfo framebufferInfo = {};
		FillAttachments (attachments, i);
		FillCreateInfo (framebufferInfo, attachments, renderPass, extent);
		if(vkCreateFramebuffer (device.GetLogicalDevice(), &framebufferInfo, allocator, &frameBuffers[i]) != VK_SUCCESS){
			ERROR ("failed to create framebuffer!");
		}
	}
	PRINT ("Frame buffers created");
}

FrameBuffers::~FrameBuffers (){
	for(auto framebuffer : frameBuffers){
		vkDestroyFramebuffer (device.GetLogicalDevice(), framebuffer, allocator);
	}
	PRINT ("Frame buffers destroyed");
}

U16 FrameBuffers::Size () const{
	return frameBuffers.size();
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

void FrameBuffers::FillAttachments (std::vector<VkImageView>& attachments, U16 index){
	attachments.push_back (swapChainImageViews[index]);
}

}
