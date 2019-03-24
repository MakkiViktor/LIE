#ifndef LIE_VK_VERTEX_BUFFER
#define LIE_VK_VERTEX_BUFFER
#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "VertexIncludes.h"
#include "LogicalDevice.h"
#include "Buffer.h"

namespace VK{

class VertexBufferMemory : public BufferMemory{
private:
	U32 GetMemoryPropertyFlags () const override final;
};

template<class VERTEX>
class VertexBuffer : public Buffer{
private:
	void FillBufferInfo (VkBufferCreateInfo& bufferInfo) override final;
	virtual BufferMemory CreateMemory (const LogicalDevice& device,
									   const VkMemoryRequirements& memRequirements,
									   VkAllocationCallbacks* allocator) const override final;
public:
	void Create (const LogicalDevice& device,const std::vector<VERTEX>& vertices, VkAllocationCallbacks* allocator = nullptr);
};



template<class VERTEX>
inline void VertexBuffer<VERTEX>::FillBufferInfo (VkBufferCreateInfo & bufferInfo){
	bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
}

template<class VERTEX>
inline BufferMemory VertexBuffer<VERTEX>::CreateMemory (const LogicalDevice & device, const VkMemoryRequirements & memRequirements, VkAllocationCallbacks * allocator) const{
	VertexBufferMemory memory;
	memory.Create (device, memRequirements, allocator);
	return memory;
}

template<class VERTEX>
inline void VertexBuffer<VERTEX>::Create (const LogicalDevice& device, const std::vector<VERTEX>& vertices, VkAllocationCallbacks* allocator){
	CreateBuffer (device, sizeof (vertices[0]) * vertices.size (), allocator);
}


}

#endif 
