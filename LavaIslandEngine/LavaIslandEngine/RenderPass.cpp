#include "RenderPass.h"

#include "SwapChain.h"

namespace VK{

RenderPass::RenderPass (const SwapChain & swapChain, const VkAllocationCallbacks* allocator):
device(swapChain.GetLogicalDevice()){
	std::vector<VkAttachmentDescription> colorAttachments;
	std::vector<VkSubpassDescription> subpasses;
	VkAttachmentReference colorAttachmentRef = {};
	FillColorAttachment (colorAttachments, swapChain.GetFormat ());
	FillSubPassDescription (subpasses, colorAttachmentRef);

	VkRenderPassCreateInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = static_cast<U32> (colorAttachments.size());
	renderPassInfo.pAttachments = colorAttachments.data();
	renderPassInfo.subpassCount = static_cast<U32> (subpasses.size ());
	renderPassInfo.pSubpasses = subpasses.data();
	if(vkCreateRenderPass (device.GetLogicalDevice(), &renderPassInfo, allocator, &renderPass) != VK_SUCCESS){
		ERROR ("failed to create render pass!");
	}

	PRINT ("RenderPass created");
}

RenderPass::~RenderPass (){
	vkDestroyRenderPass (device.GetLogicalDevice(), renderPass, allocator);
	PRINT ("RenderPass destroyed");
}

const VkRenderPass & RenderPass::GetRenderPass () const{
	return renderPass;
}

void RenderPass::FillColorAttachment (std::vector<VkAttachmentDescription>& colorAttachments, VkFormat format){
	VkAttachmentDescription colorAttachment = {};
	colorAttachment.format = format;
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	colorAttachments.push_back (colorAttachment);
}

void RenderPass::FillSubPassDescription (std::vector<VkSubpassDescription>& subpasses, VkAttachmentReference& colorAttachmentRef){

	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass = {};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;
	subpasses.push_back (subpass);
}

}

