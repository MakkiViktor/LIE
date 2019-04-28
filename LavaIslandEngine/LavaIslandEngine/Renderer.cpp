#include "Renderer.h"
#include "Extension.h"
#include "BufferTransferer.h"

namespace VK{
Renderer::Renderer (){
	LIE::Debug::InitFileName ();
	window = CreateWindow ();
	window.SetResizeCallback ([&](GLFWwindow*, int, int){
		RecreateCores();});

	instance = CreateInstance ();
	messangeGuard.Create (instance);
	surface = CreateSurface (instance,window);
	physicalDevice = CreatePhysicalDevice (instance, surface);
	logicalDevice = CreateLogicalDevice (physicalDevice);
	CreateQueues (queues, logicalDevice);
	BufferTransferer::Create (logicalDevice,queues[0].GetQueue());
	CreateRenderCores (renderCores, logicalDevice, surface);
	drawer = CreateDrawer (logicalDevice);
}

Renderer::~Renderer (){
	drawer.Destroy ();
	for(BasicRenderCore& renderCore : renderCores){
		renderCore.Destroy ();
	}

	BufferTransferer::Destroy ();
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
	RecreateCores ();
}

void Renderer::Draw (){
	glfwPollEvents ();
	for(const Queue& queue : queues){		
		if(drawer.Draw (queue, [&](U16 currentFrame){
			for(BasicRenderCore& core : renderCores){
				core.UpdateUniformBuffers (currentFrame);
			}
		   }) != VK_SUCCESS)
			RecreateCores();
	}
}

void Renderer::AddMeshes (const std::vector<MeshData<Vertex, UniformMVP>>& meshDatas){
	this->meshDatas.insert (this->meshDatas.cend(), meshDatas.cbegin(), meshDatas.cend());             
	for(BasicRenderCore& core : renderCores)
		core.AddMeshes (meshDatas);
	drawer.RefreshCommandBuffers (GetDrawDetails());
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

Drawer Renderer::CreateDrawer (const LogicalDevice& logicalDevice){
	Drawer drawer;
	drawer.Create (logicalDevice.GetLogicalDevice (), GetDrawDetails());
	return drawer;
}

void Renderer::RecreateCores (){
	for(BasicRenderCore& renderCore : renderCores)
		renderCore.Recreate (window, meshDatas);
	drawer.Destroy ();
	drawer = CreateDrawer (logicalDevice);
}

std::vector<DrawDetails> Renderer::GetDrawDetails (){
	std::vector<DrawDetails> drawDetails(renderCores.size());
	for(U16 i = 0; i < drawDetails.size (); i++){
		drawDetails[i].swapChain = renderCores[i].GetSwapChain ();
		drawDetails[i].commandBuffers = renderCores[i].GetCommandBuffers ();
	}
	return drawDetails;
}

bool Renderer::IsWindowClosed () const{
	return window.IsClosed();
}

}

