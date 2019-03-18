#include "Surface.h"

namespace VK{

const VkSurfaceKHR & Surface::GetSurface () const{
	return surface;
}

void Surface::Destroy (){
	vkDestroySurfaceKHR (instance, surface, allocator);
}

}

