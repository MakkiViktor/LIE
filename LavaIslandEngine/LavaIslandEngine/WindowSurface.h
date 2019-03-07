#ifndef LIE_VK_WIN32WINDOW_SURFICE
#define LIE_VK_WIN32WINDOW_SURFICE

#pragma once

#include "vulkan/vulkan.h"
#include "Surface.h"

namespace VK{
	
class Window;
class Instance;

class WindowSurface : public Surface{
private:
	VkSurfaceKHR surface;
	Instance& instance;
public:
	WindowSurface (Instance& instance, Window& window);
	~WindowSurface ();

	virtual const VkSurfaceKHR & GetSurface () const override final;
};

}
#endif
