#ifndef LIE_VK_STAGING_BUFFER
#define LIE_VK_STAGING_BUFFER
#pragma once

#include "Buffer.h"

namespace VK{

class StagingBufferMemory : public  BufferMemory{
private:
	U32 GetMemoryPropertyFlags () const override final;
};

class StagingBuffer : public Buffer{
private:
	void FillBufferInfo (VkBufferCreateInfo& bufferInfo) override final;
	BufferMemory CreateMemory (const LogicalDevice& device, const VkMemoryRequirements& memRequirements, VkAllocationCallbacks* allocator) const override final;
public:
	void Create (const LogicalDevice& device, SIZE memorySize, VkAllocationCallbacks* allocator = nullptr);
};

}
#endif 
