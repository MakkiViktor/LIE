#ifndef LIE_VK_DESCRIPTOR_POOL
#define LIE_VK_DESCRIPTOR_POOL
#pragma once

#include "vulkan/vulkan.h"
#include "Types.h"

namespace VK{

class SwapChain;
class DescriptorSets;

class DescriptorPool{
private:
	VkDescriptorPool descriptorPool;
	VkDevice device;
	VkAllocationCallbacks* allocator;

	virtual void FillPoolInfo (VkDescriptorPoolCreateInfo & poolInfo,
							   VkDescriptorPoolSize& poolSize,
							   U32 imageCount);
public:
	void Create (const SwapChain& swapChain, VkAllocationCallbacks* allocator = nullptr);
	void Destroy ();
	virtual void Free (const DescriptorSets& descriptorSets);

	VkDescriptorPool GetDescriptorPool () const;
};

}

#endif // !
