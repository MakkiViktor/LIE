#ifndef LIE_VK_RENDER_PASS
#define LIE_VK_RENDER_PASS

#pragma once

#include "vulkan/vulkan.h"
#include <vector>

#include "LogicalDevice.h"

namespace VK{

class SwapChain;

class RenderPass{
private:
	VkRenderPass renderPass;
	const VkAllocationCallbacks* allocator;
	const LogicalDevice& device;
public:
	RenderPass (const SwapChain& swapChain, const VkAllocationCallbacks* allocator = nullptr);
	~RenderPass ();
	const VkRenderPass& GetRenderPass () const;
protected:
	virtual void FillColorAttachment (std::vector<VkAttachmentDescription>& colorAttachments, VkFormat format);
	virtual void FillSubPassDescription (std::vector<VkSubpassDescription>& subpasses, VkAttachmentReference& colorAttachmentRef);
};
}

#endif 
