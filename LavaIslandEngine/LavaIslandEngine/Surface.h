#ifndef LIE_VK_SURFACE
#define LIE_VK_SURFACE

#pragma once

#include "vulkan/vulkan.h"

namespace VK{
class Surface{
public :
	virtual const VkSurfaceKHR& GetSurface () const = 0;
};
}

#endif 
