#ifndef LIE_VK_INDEX_BUFFER
#define LIE_VK_INDEX_BUFFER
#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "VertexIncludes.h"
#include "LogicalDevice.h"
#include "Buffer.h"

namespace VK{

class IndexBufferMemory : public BufferMemory{
private:
	U32 GetMemoryPropertyFlags () const override final;
};

class IndexBuffer : public Buffer{
private:
	U32 indexCount;

	void FillBufferInfo (VkBufferCreateInfo& bufferInfo) override final;
	BufferMemory CreateMemory (const LogicalDevice& device,
									   const VkMemoryRequirements& memRequirements,
									   VkAllocationCallbacks* allocator) const override final;
public:
	void Create (const LogicalDevice& device, const std::vector<U32>& indices, VkAllocationCallbacks* allocator = nullptr);
	U32 GetIndexCount () const;
};

}

#endif 
