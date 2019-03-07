
#define GLFW_INCLUDE_VULKAN

#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <iostream>
#include "Types.h"
#include "Window.h"
#include "Instance.h"
#include "WindowSurface.h"
#include "GLFWGuard.h"
#include "Debug.h"
#include "PhysicalDevice.h"
#include "LogicalDevice.h"
#include "Queue.h"
#include "SwapChain.h"
#include "ImageViews.h"

int main (){

	LIE::Debug::InitFileName ();

	VK::GLFWGuard glfw;{ 
	VK::Window window (LIE::DEFAULT_WIDTH, LIE::DEFAULT_HEIGHT);{
	VK::Instance instance;{
	VK::WindowSurface windowSurface (instance, window);{
	LIE::Debug::ValidationMessengerGuard messengerGuard(instance);

	VK::PhysicalDevice physicalDevice(instance,windowSurface, VK::PhysicalDeviceDetails ());
	VK::LogicalDevice logicalDevice (physicalDevice);{ 
	VK::Queue graphicsQueue(logicalDevice, physicalDevice.GetQueueFamilyIndexByFlag(VK_QUEUE_GRAPHICS_BIT));
	VK::SwapChain swapChain (windowSurface, logicalDevice, VK::SwapChainDetails (windowSurface, physicalDevice));
	VK::ImageViews imageViews (logicalDevice, swapChain, VK::ImageViewsDetails ());

	while(!window.isClosed ()){
		glfwPollEvents ();
	}
	}}}}}
	
	return 0;
}