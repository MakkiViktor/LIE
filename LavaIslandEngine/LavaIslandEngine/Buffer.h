#ifndef  LIE_VK_BUFFER
#define  LIE_VK_BUFFER
#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "VertexIncludes.h"
#include "LogicalDevice.h"

namespace VK{

class BufferMemory{
private:
	VkDeviceMemory bufferMemory = VK_NULL_HANDLE;
	VkDevice device;
	VkAllocationCallbacks* allocator;

protected:
	U32 FindMemoryType (uint32_t typeFilter, VkMemoryPropertyFlags properties, const VkPhysicalDevice& physicalDevice);
	virtual U32 GetMemoryPropertyFlags () const;
public:
	void Create (const LogicalDevice& device, const VkMemoryRequirements& memRequirements, VkAllocationCallbacks* allocator = nullptr);
	void Destroy ();

	VkDeviceMemory GetMemory () const;
};

class Buffer{
private:
	VkBuffer buffer;
	VkDevice device;
	BufferMemory memory;
	SIZE size;
	VkAllocationCallbacks* allocator;

	virtual void FillBufferInfo (VkBufferCreateInfo& bufferInfo);
	virtual BufferMemory CreateMemory (const LogicalDevice& device, const VkMemoryRequirements& memRequirements, VkAllocationCallbacks* allocator) const;
public:
	void CreateBuffer (const LogicalDevice& device, SIZE memorySize, VkAllocationCallbacks* allocator = nullptr);
	void Destroy ();
	VkBuffer GetBuffer () const;
	VkDeviceMemory GetBufferMemory () const;
	SIZE GetSize () const;
	VkDevice GetLogicalDevice () const;
};
}

#endif 
