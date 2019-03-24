#ifndef LIE_VK_BUFFER_TRANSFERER
#define LIE_VK_BUFFER_TRANSFERER
#pragma once

#include "TransferPool.h"

namespace VK{

class LogicalDevice;
class Buffer;

class BufferTransferer{
	static TransferPool transferPool;
	static VkQueue transferQueue;
public:
	static void Create (const LogicalDevice& device, VkQueue transferQueue);
	static void Destroy ();
	static void TransferBufferData (const Buffer& source, const Buffer& destination);
	static void MapMemory (const Buffer& buffer,const void* mapData);
};

}

#endif // !LIE_VK_BUFFER_TRANSFERER
