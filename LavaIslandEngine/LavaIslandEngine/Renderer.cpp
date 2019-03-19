#include "Renderer.h"
#include "Extension.h"

namespace VK{
Renderer::Renderer (){
	LIE::Debug::InitFileName ();
	window = CreateWindow ();
	window.SetResizeCallback ([&](GLFWwindow*, int, int){
		for(BasicRenderCore& renderCore : renderCores)
			renderCore.Resize (); });
	instance = CreateInstance ();
	messangeGuard.Create (instance);
	surface = CreateSurface (instance,window);
	physicalDevice = CreatePhysicalDevice (instance, surface);
	logicalDevice = CreateLogicalDevice (physicalDevice);
	CreateQueues (queues, logicalDevice);
	CreateRenderCores (renderCores, logicalDevice, surface);
}

Renderer::~Renderer (){
	vkDeviceWaitIdle (logicalDevice.GetLogicalDevice());
	for(BasicRenderCore& renderCore : renderCores){
		renderCore.Destroy ();
	}
	logicalDevice.Destroy ();
	surface.Destroy ();
	messangeGuard.Destroy ();
	instance.Destroy ();
	window.Destroy ();
}

void Renderer::AddRenderCore (const BasicRenderCore & renderCore){
	renderCores.push_back (renderCore);
}

void Renderer::SetWidthHeight (U16 width, U16 height){
	this->width = width;
	this->height = height;
}

void Renderer::Draw (){
	glfwPollEvents ();
	for(BasicRenderCore& renderCore : renderCores){
		for(const Queue& queue : queues){		
			renderCore.Draw (queue, window);
		}
	}

}

Window Renderer::CreateWindow (){
	Window window;
	window.Create (width, height);
	return window;
}

Instance Renderer::CreateInstance (){
	Instance instance;
	instance.Create (Extension::CreateDefaultExtensions());
	return instance;
}

Surface Renderer::CreateSurface (const Instance & instance, Window window){
	WindowSurface surface;
	surface.Create (instance, window);
	return surface;
}

PhysicalDevice Renderer::CreatePhysicalDevice (const Instance & instance, const Surface & surface){
	PhysicalDevice physicalDevice;
	physicalDevice.Create (instance, surface, VK::PhysicalDeviceDetails ());
	return physicalDevice;
}

LogicalDevice Renderer::CreateLogicalDevice (const PhysicalDevice & physicalDevice){
	LogicalDevice device;
	device.Create (physicalDevice);
	return device;
}

void Renderer::CreateQueues (std::vector<Queue>& queues, const LogicalDevice & logicalDevice){
	Queue queue;
	queue.Create (logicalDevice, physicalDevice.GetQueueFamilyIndexByFlag (VK_QUEUE_GRAPHICS_BIT));
	queues.push_back (queue);
}

void Renderer::CreateRenderCores (std::vector<BasicRenderCore>& renderCores, const LogicalDevice & logicalDevice, const Surface & surface){
	BasicRenderCore renderCore;
	renderCore.Create (logicalDevice, surface);
	renderCores.push_back (renderCore);
}

const Window& Renderer::GetWindow () const{
	return window;
}

}

