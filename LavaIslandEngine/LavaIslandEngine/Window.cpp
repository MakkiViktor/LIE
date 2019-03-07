#include "Window.h"

namespace VK {
Window::Window(U16 width, U16 height)
{
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	/*MV									   monitor	no relevant*/
	window = glfwCreateWindow(800, 600, "LIE", nullptr, nullptr);
}

Window::~Window()
{
	glfwDestroyWindow(window);
}

bool Window::isClosed()
{
	return glfwWindowShouldClose(window);
}

GLFWwindow * Window::GetWindow () {
	return window;
}

}
