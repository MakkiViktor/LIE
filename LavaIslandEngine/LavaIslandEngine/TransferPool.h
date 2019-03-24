#ifndef LIE_VK_TRANSFER_POOL
#define LIE_VK_TRANSFER_POOL
#pragma once

#include "CommandPool.h"

namespace VK{

class TransferCommandBuffer;

class TransferPool : public CommandPool{
	void FillCreateInfo (VkCommandPoolCreateInfo poolInfo, const PhysicalDevice& physicalDevice) override final;
public:
	void Free (TransferCommandBuffer& commandBuffers);
};
}

#endif 
