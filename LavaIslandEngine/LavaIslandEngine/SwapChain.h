#ifndef LIE_VK_SWAP_CHAIN
#define LIE_VK_SWAP_CHAIN

#pragma once

#include "LogicalDevice.h"

namespace VK{

class SwapChainDetails{
private:
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;

	VkSurfaceFormatKHR surfaceFormat;
	VkPresentModeKHR presentMode;
	VkExtent2D extent;
	U32 imageCount = 0;

	SwapChainDetails (const Surface& surface, const VkPhysicalDevice& physicalDevice);
	
	void FillFormats (const Surface& surface, const VkPhysicalDevice& physicalDevice);
	void FillPresentModes (const Surface& surface, const VkPhysicalDevice& physicalDevice);
	void FillCapabilities (const Surface& surface, const VkPhysicalDevice& physicalDevice);

	void SelectSwapSurfaceFormat ();
	void SelectSwapPresentMode ();
	void SelectExtent ();
	void SelectImageCount ();
	bool IsComplete ();
public:
	SwapChainDetails (const Surface& surface, const LogicalDevice& logicalDevice);
	VkSurfaceFormatKHR GetSurfaceFormat () const;
	VkPresentModeKHR GetPresentMode () const;
	VkExtent2D GetExtent () const;
	U32 GetImageCount () const;
	VkSurfaceCapabilitiesKHR GetCapabilities () const;

	void SetSwapSurfaceFormat (VkSurfaceFormatKHR format);
	void SetSwapPresentMode (VkPresentModeKHR mode);

	static bool IsPhysicalDeviceSuitable (const Surface& surface, const VkPhysicalDevice& device);
};

class SwapChain{
private:
	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages;
	const LogicalDevice& logicalDevice;
	const VkAllocationCallbacks* allocator;
	VkFormat format;
	VkExtent2D extent;
	U32 imageCount;
	void FillSwapChainImages ();
public:
	SwapChain (const Surface& surface, const LogicalDevice& logicalDevice, const VkAllocationCallbacks* allocator = nullptr);
	~SwapChain ();

	U16	GetImageCount () const;
	const std::vector<VkImage> GetImages () const;
	const VkSwapchainKHR& GetSwapchain () const;
	const VkFormat& GetFormat () const;
	const LogicalDevice& GetLogicalDevice () const;
	const VkExtent2D& GetExtent () const;
protected:
	virtual void FillCreateInfo (const Surface& surface,
								const PhysicalDevice& physicalDevice,
								VkSwapchainCreateInfoKHR& createInfo);

};
}

#endif 
