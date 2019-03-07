#include "GLFWGuard.h"
#include <GLFW/glfw3.h>

namespace VK {

GLFWGuard::GLFWGuard()
{
	glfwInit();
}

GLFWGuard::~GLFWGuard()
{
	glfwTerminate();
}

}
