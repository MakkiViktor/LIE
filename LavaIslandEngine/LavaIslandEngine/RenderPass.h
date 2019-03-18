#ifndef LIE_VK_RENDER_PASS
#define LIE_VK_RENDER_PASS

#pragma once

#include "LogicalDevice.h"
#include "CopyInfo.h"

namespace VK{

class SwapChain;

class RenderPass{
private:
	VkRenderPass renderPass = VK_NULL_HANDLE;
	VkAllocationCallbacks* allocator;
	VkDevice device;

	virtual void FillColorAttachment (std::vector<VkAttachmentDescription>& colorAttachments, VkFormat format);
	virtual void FillSubPassDescription (std::vector<VkSubpassDescription>& subpasses, VkAttachmentReference& colorAttachmentRef);
	virtual void FillSubpassDependency (std::vector<VkSubpassDependency>& dependencies);
public:
	void Create (const SwapChain& swapChain, VkAllocationCallbacks* allocator = nullptr);
	void Destroy ();
	const VkRenderPass& GetRenderPass () const;
};
}

#endif 
