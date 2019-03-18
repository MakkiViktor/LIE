#ifndef LIE_VK_INSTANCE
#define LIE_VK_INSTANCE

#pragma once

#include "vulkan/vulkan.h"
#include "Types.h"
#include <vector>

namespace VK{

class Extension;

class Instance{
private:
	VkInstance instance = VK_NULL_HANDLE;
	VkAllocationCallbacks* allocator;

	virtual void FillAppInfo (VkApplicationInfo& appInfo);
	virtual void FillCreateInfo (VkInstanceCreateInfo& createInfo, const VkApplicationInfo& appInfo, const Extension& extensions);
public:
	void Create (const Extension& extensions, VkAllocationCallbacks* allocator = nullptr);
	void Destroy ();
	const VkInstance& GetInstance () const;
};

}
#endif

