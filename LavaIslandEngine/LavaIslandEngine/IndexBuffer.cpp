#include "IndexBuffer.h"

namespace VK{

U32 IndexBufferMemory::GetMemoryPropertyFlags () const{
	return VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
}

void IndexBuffer::FillBufferInfo (VkBufferCreateInfo & bufferInfo){
	bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
}

BufferMemory IndexBuffer::CreateMemory (const LogicalDevice & device, const VkMemoryRequirements & memRequirements, VkAllocationCallbacks * allocator) const{
	IndexBufferMemory memory;
	memory.Create (device, memRequirements, allocator);
	return memory;
}

void IndexBuffer::Create (const LogicalDevice & device, const std::vector<U32>& indices, VkAllocationCallbacks * allocator){
	indexCount = static_cast<U32>(indices.size ());
	CreateBuffer (device, sizeof (indices[0]) * indices.size (), allocator);
}

U32 IndexBuffer::GetIndexCount () const{
	return indexCount;
}

}

