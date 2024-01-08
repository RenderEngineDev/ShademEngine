#pragma once

#include <iostream>

#include "Camera/Camera.h"
#include "glad/glad.h"
#include <GLFW/glfw3.h>


namespace Controller {

	// TODO: do zmiany dla wielu kontrolerów
	class Controller {
	public:

		// key states
		static bool isCursorHidden;

		// camera
		static bool firstMouse;
		static float lastX;
		static float lastY;

		static Camera::Camera* camera;

		Controller(Camera::Camera* camera);
		Controller() {};

		int configure(GLFWwindow* window);
		bool Controller::Controller::setCamera(Camera::Camera* camera);

		static void processKeyboardKeys(GLFWwindow* window, float deltaTime);
		static void cursorVisibilityChange(GLFWwindow* window);

		static void processInput(GLFWwindow* window);
		static void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
		static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
		static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	};
}