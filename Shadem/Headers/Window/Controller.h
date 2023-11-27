#pragma once

#include <iostream>

#include "Camera/Camera.h"
#include "glad/glad.h"
#include <GLFW/glfw3.h>


// settings
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

namespace Controller {

	// TODO: do zmiany dla wielu kontrolerów
	class Controller {
	public:

		// camera
		static float lastX;
		static float lastY;
		static bool firstMouse;

		static Camera::Camera* camera;

		Controller(Camera::Camera* camera);

		int configure(GLFWwindow* window);

		static void processInput(GLFWwindow* window);
		static void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
		static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
		static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	};
}