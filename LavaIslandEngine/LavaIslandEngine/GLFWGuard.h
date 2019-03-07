#ifndef VK_GLFW_GUARD
#define VK_GLFW_GUARD

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#pragma once

namespace VK {
class GLFWGuard
{
public:
	GLFWGuard();
	~GLFWGuard();
};

}
#endif
