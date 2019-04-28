#ifndef LIE_VK_UNIFORM_BUFFER
#define LIE_VK_UNIFORM_BUFFER
#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "LogicalDevice.h"
#include "Buffer.h"

namespace VK{

class UniformBufferMemory : public BufferMemory{
private:
	U32 GetMemoryPropertyFlags () const override final;
};

template<class UNIFORM>
class UniformBuffer : public Buffer{
private:
	void FillBufferInfo (VkBufferCreateInfo& bufferInfo) override final;
	virtual BufferMemory CreateMemory (const LogicalDevice& device,
									   const VkMemoryRequirements& memRequirements,
									   VkAllocationCallbacks* allocator) const override final;
public:
	void Create (const LogicalDevice& device, VkAllocationCallbacks* allocator = nullptr);
};

template<class UNIFORM>
inline void UniformBuffer<UNIFORM>::FillBufferInfo (VkBufferCreateInfo & bufferInfo){
	bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
}

template<class UNIFORM>
inline BufferMemory UniformBuffer<UNIFORM>::CreateMemory (const LogicalDevice & device, const VkMemoryRequirements & memRequirements, VkAllocationCallbacks * allocator) const{
	UniformBufferMemory memory;
	memory.Create (device, memRequirements, allocator);
	return memory;
}

template<class UNIFORM>
inline void UniformBuffer<UNIFORM>::Create (const LogicalDevice & device,  VkAllocationCallbacks * allocator){
	CreateBuffer (device, sizeof (UNIFORM), allocator);
}
	
}

#endif 

