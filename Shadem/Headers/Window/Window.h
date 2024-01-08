#pragma once
#include <iostream>
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

class Window {
private:
	GLFWwindow* glfwWindow = nullptr;

public:
	Window() {}

	int configure(int width, int height);
	GLFWwindow* getGLFWwindow();

	glm::vec2 getWindowSize();
};