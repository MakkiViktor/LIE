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
	void Create (U16 width, U16 height);
	void Destroy ();

	bool IsClosed() const;
	GLFWwindow* GetWindow ();
};

}

#endif 