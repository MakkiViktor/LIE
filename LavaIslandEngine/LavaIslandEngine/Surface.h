#ifndef LIE_VK_SURFACE
#define LIE_VK_SURFACE

#pragma once

#include "vulkan/vulkan.h"
#include "Types.h"

namespace VK{

class Surface{
protected:
	VkSurfaceKHR surface;
	VkInstance instance;
	VkAllocationCallbacks* allocator;
public :
	const VkSurfaceKHR& GetSurface () const;
	void Destroy ();
};

}

#endif 
