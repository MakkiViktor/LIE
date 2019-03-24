#include "StagingBuffer.h"

namespace VK{

void StagingBuffer::FillBufferInfo (VkBufferCreateInfo & bufferInfo){
	bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
}

BufferMemory StagingBuffer::CreateMemory (const LogicalDevice & device, const VkMemoryRequirements & memRequirements, VkAllocationCallbacks * allocator) const{
	StagingBufferMemory memory;
	memory.Create (device, memRequirements, allocator);
	return memory;
}

U32 StagingBufferMemory::GetMemoryPropertyFlags () const{
	return VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
}

}
