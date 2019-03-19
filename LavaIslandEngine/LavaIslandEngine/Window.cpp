#include "Window.h"
#include <algorithm>

namespace VK {
void Window::Create (U32 width, U32 height) {
	this->width = width;
	this->height = height;

	glfwWindowHint (GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint (GLFW_RESIZABLE, GLFW_TRUE);
	//#MV											 monitor	no relevant
	window = glfwCreateWindow (width, height, "LIE", nullptr, nullptr);
}

void Window::Destroy (){
	glfwDestroyWindow (window);
}

void Window::Refresh (){
	width = 0; height = 0;
	while(width == 0 || height == 0){
		glfwGetFramebufferSize (window, &width, &height);
		glfwWaitEvents ();
	}
}

void Window::SetResizeCallback (static std::function< void (GLFWwindow*window, int width, int height)> callback){
	resizeCallback = callback;
	glfwSetWindowUserPointer (window, this);
	glfwSetFramebufferSizeCallback (window, framebufferResizeCallback);
}

void Window::framebufferResizeCallback (GLFWwindow * window, int width, int height){
	auto app = reinterpret_cast<Window*>(glfwGetWindowUserPointer (window));
	app->resizeCallback (window, width, height);
}

bool Window::IsClosed() const{
	return glfwWindowShouldClose(window);
}

GLFWwindow * Window::GetWindow () {
	return window;
}

}
