#include "UniformBuffer.h"

U32 VK::UniformBufferMemory::GetMemoryPropertyFlags () const{
	return VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
}
