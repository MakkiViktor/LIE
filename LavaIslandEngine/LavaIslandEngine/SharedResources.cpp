#include "SharedResources.h"

VkExtent2D VK::GetSharedWindowExtent (){
	int width, height;
	glfwGetFramebufferSize (sharedWindow.GetWindow(), &width, &height);

	VkExtent2D actualExtent = {
			static_cast<uint32_t>(width),
			static_cast<uint32_t>(height)
	};
	return VkExtent2D ();
}
