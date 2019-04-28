#include "BufferTransferer.h"

#include "TransferCommandBuffer.h"
#include "Buffer.h"

namespace VK{

TransferPool BufferTransferer::transferPool;
VkQueue BufferTransferer::transferQueue;
LogicalDevice BufferTransferer::logicalDevice;

void BufferTransferer::Create (const LogicalDevice& device,VkQueue transferQueue){
	transferPool.Create (device);
	BufferTransferer::transferQueue = transferQueue;
	BufferTransferer::logicalDevice = device;
}

void BufferTransferer::Destroy (){
	transferPool.Destroy ();
}

void BufferTransferer::TransferBufferData (const Buffer& source, const Buffer& destination){

	TransferCommandBuffer transferCommand;
	transferCommand.Create (transferPool, source.GetBuffer(), destination.GetBuffer(), source.GetSize());

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &transferCommand.GetCommandBuffer();

	vkQueueSubmit (transferQueue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle (transferQueue);
	transferPool.Free (transferCommand);
}

void BufferTransferer::MapMemory (const Buffer & buffer, const void * mapData){
	void* data;
	vkMapMemory (transferPool.GetLogicalDevice (), buffer.GetBufferMemory (), 0, buffer.GetSize(), 0, &data);
	memcpy (data, mapData, buffer.GetSize ());
	vkUnmapMemory (transferPool.GetLogicalDevice (), buffer.GetBufferMemory ());
}

}
