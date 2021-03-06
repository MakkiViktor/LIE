#include "SwapChain.h"

#include "Surface.h"
#include "StructureOperations.h"
#include "Debug.h"
#include "SharedResources.h"
#include <algorithm>

namespace VK{

//SwapChainDetails
SwapChainDetails::SwapChainDetails (const Surface & surface, const VkPhysicalDevice & physicalDevice){
	FillCapabilities (surface, physicalDevice);
	FillFormats (surface, physicalDevice);
	FillPresentModes (surface, physicalDevice);

	SelectSwapSurfaceFormat ();
	SelectSwapPresentMode ();
	SelectExtent ();
	SelectImageCount ();
}


VkSurfaceFormatKHR SwapChainDetails::GetSurfaceFormat () const{
	return surfaceFormat;
}

VkPresentModeKHR SwapChainDetails::GetPresentMode () const{
	return presentMode;
}

VkExtent2D SwapChainDetails::GetExtent () const{
	return extent;
}

U32 SwapChainDetails::GetImageCount () const{
	return imageCount;
}

VkSurfaceCapabilitiesKHR SwapChainDetails::GetCapabilities () const{
	return capabilities;
}


void SwapChainDetails::SetSwapSurfaceFormat (VkSurfaceFormatKHR format){
	if(SO::Contains (formats, format))
		surfaceFormat = format;
	else
		WARNING ("format not supported on this physical device");
}

void SwapChainDetails::SetSwapPresentMode (VkPresentModeKHR mode){
	if(SO::Contains (presentModes, mode))
		presentMode = mode;
	else
		WARNING ("present mode not supported on this physical device");
}

bool SwapChainDetails::IsPhysicalDeviceSuitable (const Surface& surface, const VkPhysicalDevice& device){
	SwapChainDetails swapChain (surface, device);
	return swapChain.IsComplete ();
}

void SwapChainDetails::FillFormats (const Surface & surface, const VkPhysicalDevice & physicalDevice){
	U32 formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR (physicalDevice,
										  surface.GetSurface (),
										  &formatCount,
										  nullptr);

	if(formatCount != 0){
		formats.resize (formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR (physicalDevice,
											  surface.GetSurface (),
											  &formatCount,
											  formats.data ());
	}
}

void SwapChainDetails::FillPresentModes (const Surface & surface, const VkPhysicalDevice & physicalDevice){
	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR (physicalDevice,
											   surface.GetSurface (),
											   &presentModeCount,
											   nullptr);
	if(presentModeCount != 0){
		presentModes.resize (presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR (physicalDevice,
												   surface.GetSurface (),
												   &presentModeCount,
												   presentModes.data ());
	}
}

void SwapChainDetails::FillCapabilities (const Surface & surface, const VkPhysicalDevice & physicalDevice){
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR (physicalDevice,
											   surface.GetSurface (),
											   &capabilities);
}


bool SwapChainDetails::IsComplete (){
	return !formats.empty () && !presentModes.empty ();
}

void SwapChainDetails::SelectSwapSurfaceFormat (){
	if(formats.size () == 1 && formats[0].format == VK_FORMAT_UNDEFINED){
		surfaceFormat = {VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
	}

	for(const auto& availableFormat : formats){
		if(availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR){
			surfaceFormat = availableFormat;
		}
	}
	surfaceFormat = formats[0];
}

void SwapChainDetails::SelectSwapPresentMode (){
	SetSwapPresentMode (VK_PRESENT_MODE_FIFO_KHR);
	SetSwapPresentMode (VK_PRESENT_MODE_IMMEDIATE_KHR);
	SetSwapPresentMode (VK_PRESENT_MODE_MAILBOX_KHR);
}

void SwapChainDetails::SelectExtent (){
	if(capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max ()){
		extent = capabilities.currentExtent;
	}
	else{
		VkExtent2D actualExtent = GetSharedWindowExtent();
		actualExtent.width = std::max (capabilities.minImageExtent.width, std::min (capabilities.maxImageExtent.width, actualExtent.width));
		actualExtent.height = std::max (capabilities.minImageExtent.height, std::min (capabilities.maxImageExtent.height, actualExtent.height));
		extent = actualExtent;
	}

}

void SwapChainDetails::SelectImageCount (){
	imageCount = capabilities.minImageCount + 1;
	if(capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount){
		imageCount = capabilities.maxImageCount;
	}
}

//SwapChain

void SwapChain::Create (const LogicalDevice& logicalDevice, const Surface& surface, VkAllocationCallbacks* allocator){
	device = logicalDevice.GetLogicalDevice ();
	this->allocator = allocator;
	VkSwapchainCreateInfoKHR createInfo = {};
	FillCreateInfo (surface, logicalDevice.GetPhysicalDevice (), createInfo);
	format = createInfo.imageFormat;
	extent = createInfo.imageExtent;
	imageCount = createInfo.minImageCount;
	if(vkCreateSwapchainKHR (logicalDevice.GetLogicalDevice (), &createInfo, allocator, &swapChain) != VK_SUCCESS){
		ERROR ("failed to create swap chain!");
	}
	LIE::Debug::Print ("Swap chain created");

	FillSwapChainImages ();
}

void SwapChain::Destroy (){
	if(swapChain != VK_NULL_HANDLE){
		vkDestroySwapchainKHR (device, swapChain, allocator);
		swapChain = VK_NULL_HANDLE;
		PRINT ("Swap chain destroyed");
	}
}

U16 SwapChain::GetImageCount () const{
	return imageCount;
}

const std::vector<VkImage> SwapChain::GetImages () const{
	return swapChainImages;
}

const VkSwapchainKHR & SwapChain::GetSwapchain () const{
	return swapChain;
}

const VkFormat & SwapChain::GetFormat () const{
	return format;
}

VkDevice SwapChain::GetLogicalDevice () const{
	return device;
}

const VkExtent2D & SwapChain::GetExtent () const{
	return extent;
}

void SwapChain::FillCreateInfo (const Surface& surface, 
								const PhysicalDevice& physicalDevice, 
								VkSwapchainCreateInfoKHR & createInfo){
	SwapChainDetails details (surface, physicalDevice.GetPhysicalDevice());
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = surface.GetSurface ();
	createInfo.minImageCount = details.GetImageCount ();
	createInfo.imageFormat = details.GetSurfaceFormat ().format;
	createInfo.imageColorSpace = details.GetSurfaceFormat ().colorSpace;
	createInfo.imageExtent = details.GetExtent ();
	//TODO change if more render image
	createInfo.imageArrayLayers = 1;
	//VK_IMAGE_USAGE_TRANSFER_DST_BIT if target rendering
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	createInfo.preTransform = details.GetCapabilities ().currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = details.GetPresentMode ();
	createInfo.clipped = VK_TRUE;

	std::vector<U32> queueFamilyIndices = physicalDevice.GetQueueFamilyIndicesArray ();

	if(queueFamilyIndices.size () > 1){
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = static_cast<U32>(queueFamilyIndices.size ());
		createInfo.pQueueFamilyIndices = queueFamilyIndices.data ();
	}
	else{
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0; // Optional
	}
}

void SwapChain::FillSwapChainImages (){
	vkGetSwapchainImagesKHR (device, swapChain, &imageCount, nullptr);
	swapChainImages.resize (imageCount);
	vkGetSwapchainImagesKHR (device, swapChain, &imageCount, swapChainImages.data ());
}

}