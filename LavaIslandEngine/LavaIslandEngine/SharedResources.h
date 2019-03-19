#ifndef LIE_VK_SHARED_RESOURCES
#define LIE_VK_SHARED_RESOURCES
#pragma once

#include "Window.h"

namespace VK{
static Window sharedWindow;

VkExtent2D GetSharedWindowExtent();

}

#endif 
