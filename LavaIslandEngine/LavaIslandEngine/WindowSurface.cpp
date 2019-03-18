#include "WindowSurface.h"

#include "Window.h"
#include "Instance.h"
#include "GLFW/glfw3.h"



namespace VK{
void WindowSurface::Create (const Instance& instance, Window& window, VkAllocationCallbacks* allocator){
	this->allocator = allocator;
	this->instance = instance.GetInstance ();
	if(glfwCreateWindowSurface (this->instance, window.GetWindow (), allocator, &surface) != VK_SUCCESS){
		ERROR ("failed to create window surface!");
	}
}

}

