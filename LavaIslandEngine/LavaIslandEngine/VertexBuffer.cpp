#include "VertexBuffer.h"

#include "SharedResources.h"

namespace VK{

U32 VertexBufferMemory::GetMemoryPropertyFlags () const{
	return VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
}
}
