#ifndef LIE_VK_WIN32WINDOW_SURFICE
#define LIE_VK_WIN32WINDOW_SURFICE

#pragma once

#include "vulkan/vulkan.h"
#include "Surface.h"

namespace VK{
	
class Window;
class Instance;

class WindowSurface : public Surface{
public:
	void Create (const Instance& instance, Window& window, VkAllocationCallbacks* allocator = nullptr);

};

}
#endif
