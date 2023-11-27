#pragma once
#include <iostream>
#include <Window/Controller.h>

class Window {
private:
	GLFWwindow* glfwWindow = nullptr;

public:
	Window() {}

	int configure();
	GLFWwindow* getGLFWwindow();
};