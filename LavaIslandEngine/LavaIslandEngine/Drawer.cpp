#include "Drawer.h"
#include "Window.h"
#include "Queue.h"
#include "SwapChain.h"

namespace VK{


void Drawer::Create (const VkDevice& device, const std::vector<DrawDetails>& drawDetails){
	this->device = device;
	this->drawDetails = drawDetails;
	this->commandBufferOffset = GetMaxCommandBufferSize ();
	this->commanBuffers.resize (commandBufferOffset * drawDetails.size ());
	this->imageIndexes.resize (drawDetails.size ());
	frameSyncObjects.resize (maxFramesInFlight);

	CopyFrameSwapChains ();
	CreateFrameObjects ();
	CreateFences ();
	PRINT ("Drawer created");
}

void Drawer::Destroy (){
	vkDeviceWaitIdle (device);
	for(DrawDetails& detail : drawDetails)
		detail.Destroy ();
	for(Fence& fence : inFlightFences)
		fence.Destroy ();
	PRINT ("Drawer destroyed");
}

VkResult Drawer::Draw (const Queue& queue, std::function<void (U16 currentFrame)> update){
	VkResult result;

	vkWaitForFences (device,
					 1,
					 &inFlightFences[currentFrame].GetFence(),
					 VK_TRUE, std::numeric_limits<uint64_t>::max ());

	result = AquireAllImages (update);
	if(result != VK_SUCCESS)
		return result;

	SubmitQueue (queue);
	result = PresentQueue (queue);
	currentFrame = (currentFrame + 1) % maxFramesInFlight;
	return result;
}

void Drawer::RefreshCommandBuffers (const std::vector<DrawDetails>& drawDetails){
	for(U16 i = 0; i < this->drawDetails.size (); i++){
		this->drawDetails[i].commandBuffers = drawDetails[i].commandBuffers;
	}
	this->commandBufferOffset = GetMaxCommandBufferSize ();
	this->commanBuffers.resize (commandBufferOffset * drawDetails.size ());
}

void Drawer::RefreshCommandBuffers (){
	for(U16 i = 0; i < imageIndexes.size(); i++){
		for(U16 j = 0; j < commandBufferOffset; j++)
		commanBuffers[i * commandBufferOffset + j] = drawDetails[i].commandBuffers[j][imageIndexes[i]];
	}
}

SIZE Drawer::GetMaxCommandBufferSize (){
	SIZE size = 0;
	for(DrawDetails& detail: drawDetails){
		SIZE s = detail.commandBuffers.size ();
		if( s > size)
			size = s;
	}
	return size;
}

bool Drawer::IsValidDetail (const DrawDetails & details){
	return details.swapChain != VK_NULL_HANDLE;
}

void Drawer::CopyFrameSwapChains (){
	swapChains.resize (drawDetails.size ());
	for(U16 drawIndex = 0; drawIndex < drawDetails.size (); drawIndex++){
		if(!IsValidDetail (this->drawDetails[drawIndex]))
			ERROR ("Not valid draw details");
		this->drawDetails[drawIndex].Create (device, maxFramesInFlight);
		swapChains[drawIndex] = this->drawDetails[drawIndex].swapChain;
	}
}

void Drawer::CreateFrameObjects (){
	for(U16 frameIndex = 0; frameIndex < maxFramesInFlight; frameIndex++){
		frameSyncObjects[frameIndex].imageAvailableSemaphores.resize (drawDetails.size ());
		frameSyncObjects[frameIndex].renderFinishedSemaphores.resize (drawDetails.size ());
		for(U16 drawIndex = 0; drawIndex < drawDetails.size (); drawIndex++){
			frameSyncObjects[frameIndex].imageAvailableSemaphores[drawIndex] = drawDetails[drawIndex].imageAvailableSemaphores[frameIndex].GetSemaphore ();
			frameSyncObjects[frameIndex].renderFinishedSemaphores[drawIndex] = drawDetails[drawIndex].renderFinishedSemaphores[frameIndex].GetSemaphore ();	
		}
	}
}

void Drawer::CreateFences (){
	inFlightFences.resize (maxFramesInFlight);
	for(Fence& fence : inFlightFences)
		fence.Create (device);
}


VkResult Drawer::AquireImage (const VkSwapchainKHR & swapChain, U32& imageIndex, U16 swapChainIndex){
	VkResult result = vkAcquireNextImageKHR (device,
											 swapChain,
											 std::numeric_limits<uint64_t>::max (),
											 drawDetails[swapChainIndex].imageAvailableSemaphores[currentFrame].GetSemaphore (),
											 VK_NULL_HANDLE,
											 &imageIndex);

	if(result == VK_ERROR_OUT_OF_DATE_KHR){
		return result;
	}
	else if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR){
		ERROR ("failed to acquire swap chain image!");
	}
	return result;
}

VkResult Drawer::AquireAllImages (std::function<void (U16 currentFrame)> update){
	for(U16 swapChainIndex = 0; swapChainIndex < drawDetails.size (); swapChainIndex++){
		if(AquireImage (drawDetails[swapChainIndex].swapChain, imageIndexes[swapChainIndex], swapChainIndex) != VK_SUCCESS)
			return VK_ERROR_OUT_OF_DATE_KHR;
		update (imageIndexes[swapChainIndex]);
	}
	return VK_SUCCESS;
}

VkResult Drawer::PresentQueue (const Queue & queue){
	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = static_cast<U32>(frameSyncObjects[currentFrame].renderFinishedSemaphores.size ());
	presentInfo.pWaitSemaphores = frameSyncObjects[currentFrame].renderFinishedSemaphores.data ();
	presentInfo.swapchainCount = static_cast<U32>(swapChains.size ());
	presentInfo.pSwapchains = swapChains.data ();
	presentInfo.pImageIndices = imageIndexes.data();

	VkResult result = vkQueuePresentKHR (queue.GetQueue (), &presentInfo);
	
	if(result != VK_SUCCESS){
		ERROR ("failed to present swap chain image!");
	}
	return result;
}

void Drawer::SubmitQueue (const Queue & queue){
	RefreshCommandBuffers ();
	
	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
	submitInfo.waitSemaphoreCount = static_cast<U32>(frameSyncObjects[currentFrame].imageAvailableSemaphores.size ());
	submitInfo.pWaitSemaphores = frameSyncObjects[currentFrame].imageAvailableSemaphores.data ();
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = static_cast<U32>(commanBuffers.size());
	submitInfo.pCommandBuffers = commanBuffers.data();
	submitInfo.signalSemaphoreCount = static_cast<U32>(frameSyncObjects[currentFrame].renderFinishedSemaphores.size ());
	submitInfo.pSignalSemaphores = frameSyncObjects[currentFrame].renderFinishedSemaphores.data ();
	
	vkResetFences (device, 1, &inFlightFences[currentFrame].GetFence ());

	if(vkQueueSubmit (queue.GetQueue (), 1, &submitInfo, inFlightFences[currentFrame].GetFence ()) != VK_SUCCESS){
		ERROR ("failed to submit draw command buffer!");
	}
}

void DrawDetails::Create (const VkDevice & device, U16 maxFramesInFlight){
	for(U16 i = 0; i < maxFramesInFlight; i++){
		Semaphore renderFinishedSemaphore;
		Semaphore imageAvailableSemaphore;
		renderFinishedSemaphore.Create (device);
		imageAvailableSemaphore.Create (device);
		renderFinishedSemaphores.push_back (renderFinishedSemaphore);
		imageAvailableSemaphores.push_back (imageAvailableSemaphore);
	}
}

void DrawDetails::Destroy (){
	for(Semaphore& semaphore : renderFinishedSemaphores)
		semaphore.Destroy ();
	for(Semaphore& semaphore : imageAvailableSemaphores)
		semaphore.Destroy ();
}

}
