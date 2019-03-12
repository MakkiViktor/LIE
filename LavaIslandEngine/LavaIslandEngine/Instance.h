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
	VkInstance instance;
	const VkAllocationCallbacks* allocator;
public:
	Instance ();
	Instance (const Instance&) = delete;
	Instance (const Extension& extensions, const VkAllocationCallbacks* allocator = nullptr);
	~Instance ();
	VkInstance& GetInstance ();
protected:
	virtual void FillAppInfo (VkApplicationInfo& appInfo);
	virtual void FillCreateInfo (VkInstanceCreateInfo& createInfo, const VkApplicationInfo& appInfo, const Extension& extensions);
};

}
#endif

