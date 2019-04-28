#ifndef LIE_VK_BUFFER_TRANSFERER
#define LIE_VK_BUFFER_TRANSFERER
#pragma once

#include "TransferPool.h"
#include "StagingBuffer.h"
#include "LogicalDevice.h"

namespace VK{

class BufferTransferer{
private:
	static TransferPool transferPool;
	static VkQueue transferQueue;
	static LogicalDevice logicalDevice;
public:
	static void Create (const LogicalDevice& device, VkQueue transferQueue);
	static void Destroy ();
	static void TransferBufferData (const Buffer& source, const Buffer& destination);
	static void MapMemory (const Buffer& buffer,const void* mapData);
	
	template<class DataType>
	static void TransferDataToBuffer (const std::vector<DataType>& data, const Buffer& buffer);
};

template<class DataType>
inline void BufferTransferer::TransferDataToBuffer (const std::vector<DataType>& data, const Buffer & buffer){
	StagingBuffer stagingBuffer;
	stagingBuffer.Create (BufferTransferer::logicalDevice, buffer.GetSize ());
	BufferTransferer::MapMemory (stagingBuffer, data.data ());
	BufferTransferer::TransferBufferData (stagingBuffer, buffer);
	stagingBuffer.Destroy ();
}

}

#endif 
