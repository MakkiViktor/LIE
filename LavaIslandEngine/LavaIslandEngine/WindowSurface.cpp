#include "WindowSurface.h"

#include "Window.h"
#include "Instance.h"
#include "GLFW/glfw3.h"



namespace VK{

//TODO dont copy the instance, glfw
WindowSurface::WindowSurface (Instance& instance, Window& window): instance(instance){
	if(glfwCreateWindowSurface (instance.GetInstance(), window.GetWindow(), nullptr, &surface) != VK_SUCCESS){
		ERROR("failed to create window surface!");
	}
}

WindowSurface::~WindowSurface (){
	vkDestroySurfaceKHR (instance.GetInstance(), surface, nullptr);
}

const VkSurfaceKHR & WindowSurface::GetSurface () const{
	return surface;
}

}

