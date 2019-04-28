#ifndef LIE_VK_RENDERER
#define LIE_VK_RENDERER
#pragma once

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
#include "BasicRenderCore.h"
#include "Drawer.h"

namespace VK{


class Renderer{
private:
	U16 width = 800;
	U16 height = 600;

	std::vector<MeshData<Vertex, UniformMVP>> meshDatas;
	GLFWGuard glfw;
	Window window;
	Instance instance;
	LIE::Debug::ValidationMessengerGuard messangeGuard;
	Surface surface;
	PhysicalDevice physicalDevice;
	LogicalDevice logicalDevice;
	std::vector<Queue> queues;
	std::vector<BasicRenderCore> renderCores;
	Drawer drawer;

	virtual Window CreateWindow ();
	virtual Instance CreateInstance ();
	virtual Surface CreateSurface (const Instance& instance, Window window);
	virtual PhysicalDevice CreatePhysicalDevice (const Instance& instance, const Surface & surface);
	virtual LogicalDevice CreateLogicalDevice (const PhysicalDevice& physicalDevice);
	virtual void CreateQueues (std::vector<Queue>& queues, const LogicalDevice& logicalDevice);
	virtual void CreateRenderCores (std::vector<BasicRenderCore>& renderCores, const LogicalDevice& logicalDevice, const Surface& surface);
	virtual Drawer CreateDrawer (const LogicalDevice& logicalDevice);

	void RecreateCores ();
protected:
	std::vector<DrawDetails> GetDrawDetails ();

public:
	Renderer ();
	~Renderer ();
	void AddRenderCore (const BasicRenderCore& renderCore);
	void SetWidthHeight (U16 width, U16 height);

	bool IsWindowClosed() const;

	virtual void Draw ();
	void AddMeshes (const std::vector<MeshData<Vertex, UniformMVP>>& meshDatas);
};	


}

#endif 
