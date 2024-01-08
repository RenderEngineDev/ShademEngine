#pragma once

#include "Window/Window.h"

int Window::configure(int width, int height) {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindow = glfwCreateWindow(width, height, "Shadem", NULL, NULL);
	glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glfwSwapInterval(0);

	if (glfwWindow == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	return 1;
}


GLFWwindow* Window::getGLFWwindow() {
	return glfwWindow;
}

glm::vec2 Window::getWindowSize()
{
	if (glfwWindow) {
		int width, height;
		glfwGetWindowSize(glfwWindow, &width, &height);
		return glm::vec2(width, height);
	}
	return glm::vec2();
}
