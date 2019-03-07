#ifndef VK_WINDOW
#define VK_WINDOW

#pragma once

#include <GLFW/glfw3.h>
#include "Types.h"

namespace VK {

class Window
{
private:
	GLFWwindow* window;
public:
	Window(U16 width, U16 height);
	~Window();

	bool isClosed();
	GLFWwindow* GetWindow () ;
};

}

#endif 