#include "Window.h"

namespace VK {

void Window::Create (U16 width, U16 height){
	glfwWindowHint (GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint (GLFW_RESIZABLE, GLFW_FALSE);
	/*MV									   monitor	no relevant*/
	window = glfwCreateWindow (width, height, "LIE", nullptr, nullptr);
}

void Window::Destroy (){
	glfwDestroyWindow (window);
}

bool Window::IsClosed() const{
	return glfwWindowShouldClose(window);
}

GLFWwindow * Window::GetWindow () {
	return window;
}

}
