#include "DescriptorPool.h"
#include "SwapChain.h"
#include "DescriptorSets.h"

namespace VK{
void DescriptorPool::Create (const SwapChain & swapChain, VkAllocationCallbacks * allocator){
	this->device = swapChain.GetLogicalDevice ();
	this->allocator = allocator;
	VkDescriptorPoolSize poolSize = {};
	VkDescriptorPoolCreateInfo poolInfo = {};

	FillPoolInfo (poolInfo, poolSize, swapChain.GetImageCount ());

	if(vkCreateDescriptorPool (device, &poolInfo, allocator, &descriptorPool) != VK_SUCCESS){
		ERROR ("failed to create descriptor pool!");
	}
	PRINT ("Descriptor pool created");
}

void DescriptorPool::Destroy (){
	vkDestroyDescriptorPool (device, descriptorPool, allocator);
}

void DescriptorPool::Free (const DescriptorSets& descriptorSets){
	vkFreeDescriptorSets (device,
						  descriptorPool,
						  descriptorSets.Size (),
						  descriptorSets.Data ());
	PRINT("Descriptor set freed from pool");
}

VkDescriptorPool DescriptorPool::GetDescriptorPool () const{
	return descriptorPool;
}

void DescriptorPool::FillPoolInfo (VkDescriptorPoolCreateInfo & poolInfo, VkDescriptorPoolSize& poolSize, U32 imageCount){
	poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSize.descriptorCount = imageCount;

	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = 1;
	poolInfo.pPoolSizes = &poolSize;
	poolInfo.maxSets = imageCount;
}

}

