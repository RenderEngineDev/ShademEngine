#pragma once
#include "Window/Controller.h"
#include "ShademApplication/ShademEngine.h"

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------

// camera
bool Controller::Controller::isCursorHidden = false;
bool Controller::Controller::firstMouse = true;
float Controller::Controller::lastX = 0.0f;
float Controller::Controller::lastY = 0.0f;

Camera::Camera* Controller::Controller::camera = nullptr;

Controller::Controller::Controller(Camera::Camera* camera) {
	Controller::camera = camera;
};

bool Controller::Controller::setCamera(Camera::Camera* camera) {
	if (camera) {
		Controller::camera = camera;
		return true;
	} 
	return false;
}

int Controller::Controller::configure(GLFWwindow* window) {
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwSwapInterval(0);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	return 1;
}

void Controller::Controller::processInput(GLFWwindow* window) {
	float deltaTime = ShademEngine::deltaTime;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	cursorVisibilityChange(window);
	processKeyboardKeys(window, deltaTime);
}

void Controller::Controller::cursorVisibilityChange(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS && isCursorHidden) {
		isCursorHidden = false;

		if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		else {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		}
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE) {
		isCursorHidden = true;
	}
}

void Controller::Controller::processKeyboardKeys(GLFWwindow* window, float deltaTime) {
	if (!camera) {
		return;
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera->ProcessKeyboard(Camera::FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera->ProcessKeyboard(Camera::BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera->ProcessKeyboard(Camera::LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera->ProcessKeyboard(Camera::RIGHT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		camera->ProcessKeyboard(Camera::UP, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		camera->ProcessKeyboard(Camera::DOWN, deltaTime);
	}
}


void Controller::Controller::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	if (camera && (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED || glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)) { // IF CURSOR IS HIDE
		camera->ProcessMouseMovement(xoffset, yoffset);
	}
}

void Controller::Controller::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (camera && glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) { // IF CURSOR IS HIDE
		camera->ProcessMouseScroll(static_cast<float>(yoffset));
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void Controller::Controller::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.

	glViewport(0, 0, width, height);
}
