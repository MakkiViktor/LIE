#ifndef VK_WINDOW
#define VK_WINDOW

#pragma once

#include <GLFW/glfw3.h>
#include "Types.h"
#include <functional>

namespace VK {

class Window
{
private:
	GLFWwindow* window;
	I32 width = 0, height = 0;
	std::function<void (GLFWwindow* window, int width, int height)> resizeCallback;

	static void framebufferResizeCallback (GLFWwindow* window, int width, int height);
public:
	void Create (U32 width, U32 height);
	void Destroy ();
	void Refresh ();
	void SetResizeCallback (std::function<void (GLFWwindow* window, int width, int height)> callback);
	bool IsClosed() const;
	GLFWwindow* GetWindow ();
};

}

#endif 