#define LIE_VK_UNIFORM_BUFFER
#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "SwapChain.h"
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
class UniformBuffers{
private:
	std::vector<UniformBuffer<UNIFORM>> uniformBuffers;
public:
	void Create (const LogicalDevice & logicalDevice,
				 const SwapChain & swapChain);
	void Destroy ();
	
	const std::vector<UniformBuffer<UNIFORM>>& GetUniformBuffers () const;
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
	
template<class UNIFORM>
inline void UniformBuffers<UNIFORM>::Create (const LogicalDevice & logicalDevice, const SwapChain & swapChain){
	U16 imageCount = swapChain.GetImageCount ();
	uniformBuffers.resize (imageCount);
	for(U16 imageIndex = 0; imageIndex < imageCount; imageIndex++){
		UniformBuffer<UNIFORM> uniformBuffer;
		uniformBuffer.Create (logicalDevice);
		uniformBuffers[imageIndex] = uniformBuffer;
	}
}

template<class UNIFORM>
inline void UniformBuffers<UNIFORM>::Destroy (){
	for(Buffer& buffer : uniformBuffers)
		buffer.Destroy ();
}

template<class UNIFORM>
inline const std::vector<UniformBuffer<UNIFORM>>& UniformBuffers<UNIFORM>::GetUniformBuffers () const{
	return uniformBuffers;
}

}

