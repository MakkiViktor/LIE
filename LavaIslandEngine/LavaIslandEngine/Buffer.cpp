#include "Buffer.h"

namespace VK{

//Buffer

void Buffer::FillBufferInfo (VkBufferCreateInfo & bufferInfo){
	bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
}

BufferMemory Buffer::CreateMemory (const LogicalDevice & device, const VkMemoryRequirements & memRequirements, VkAllocationCallbacks * allocator) const{
	BufferMemory memory;
	memory.Create (device, memRequirements, allocator);
	return BufferMemory ();
}


void Buffer::CreateBuffer (const LogicalDevice & device, SIZE memorySize, VkAllocationCallbacks * allocator){
	this->device = device.GetLogicalDevice ();
	this->allocator = allocator;
	size = memorySize;
	VkBufferCreateInfo bufferInfo = {};
	FillBufferInfo (bufferInfo);
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = memorySize;
	if(vkCreateBuffer (device.GetLogicalDevice (), &bufferInfo, allocator, &buffer) != VK_SUCCESS){
		ERROR ("failed to create vertex buffer!");
	}
	PRINT ("Buffer Created");

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements (device.GetLogicalDevice (), buffer, &memRequirements);
	memory = CreateMemory (device, memRequirements, allocator);
	vkBindBufferMemory (device.GetLogicalDevice (), buffer, memory.GetMemory (), 0);
}

void Buffer::Destroy (){
	memory.Destroy ();
	vkDestroyBuffer (device, buffer, allocator);
	PRINT ("Buffer Destroyed");
}

VkBuffer Buffer::GetBuffer () const{
	return buffer;
}

VkDeviceMemory Buffer::GetBufferMemory () const{
	return memory.GetMemory();
}

SIZE Buffer::GetSize () const{
	return size;
}

//BufferMemory

U32 BufferMemory::GetMemoryPropertyFlags () const{
	return VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
}

U32 BufferMemory::FindMemoryType (uint32_t typeFilter,
								  VkMemoryPropertyFlags properties,
								  const VkPhysicalDevice & physicalDevice){

	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties (physicalDevice, &memProperties);

	for(uint32_t i = 0; i < memProperties.memoryTypeCount; i++){
		if((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties){
			return i;
		}
	}
	ERROR ("failed to find suitable memory type!");
	return VK_ERROR_MEMORY_MAP_FAILED;
}

void BufferMemory::Create (const LogicalDevice & device,
						   const VkMemoryRequirements & memRequirements,
						   VkAllocationCallbacks * allocator){

	this->device = device.GetLogicalDevice ();
	this->allocator = allocator;
	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = FindMemoryType (memRequirements.memoryTypeBits,
												GetMemoryPropertyFlags(),
												device.GetPhysicalDevice ().GetPhysicalDevice ());

	if(vkAllocateMemory (device.GetLogicalDevice (), &allocInfo, allocator, &bufferMemory) != VK_SUCCESS){
		ERROR ("failed to allocate vertex buffer memory!");
	}
	PRINT ("Memory allocated for Buffer " + std::to_string (memRequirements.size) + " bytes");
}

void BufferMemory::Destroy (){
	vkFreeMemory (device, bufferMemory, allocator);
	PRINT ("Buffer Memory deallocated");
}

VkDeviceMemory BufferMemory::GetMemory () const{
	return bufferMemory;
}

}

