#include "Semaphore.h"

namespace VK{

void Semaphore::FillCreateInfo (VkSemaphoreCreateInfo& semaphoreInfo){
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
}

void Semaphore::Create (const LogicalDevice & device, VkAllocationCallbacks * allocator){
	this->device = device.GetLogicalDevice ();
	this->allocator = allocator;
	VkSemaphoreCreateInfo semaphoreInfo = {};
	if(vkCreateSemaphore (this->device, &semaphoreInfo, allocator, &semaphore) != VK_SUCCESS)
		ERROR ("failed to create semaphore!");
	PRINT ("Semaphor created");
}

void Semaphore::Destroy (){
	if(semaphore != VK_NULL_HANDLE){
		vkDestroySemaphore (device, semaphore, allocator);
		PRINT ("Semaphore destroyed");
	}
}

VkSemaphore Semaphore::GetSemaphore () const{
	return semaphore;
}

}

