#pragma once

#include "ShademApplication/ShademEngine.h"

#include <iostream>
#include "Camera/Camera.h"
#include "Shader/Shader.h"
#include "Objects/Primitives.h"
#include "CubeMarching/CmObject.h"

// settings
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

// timing
float ShademEngine::lastFrame = 0.0f;
float ShademEngine::deltaTime = 0.0f;

ShademEngine::ShademEngine() {
	configure();
}

int ShademEngine::run() {
	Shader shader = Shader("../Shadem/Shaders/BasicShader/Vshader.glsl", "../Shadem/Shaders/BasicShader/Fshader.glsl");

	while (!glfwWindowShouldClose(window->getGLFWwindow())) {
		glClearColor(0.2f, 0.3f, 0.7f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		calculateFrameTime();
		controller->processInput(window->getGLFWwindow());

		// render
		scene->draw(shader);
		
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window->getGLFWwindow());
		glfwPollEvents();
	}
	glfwTerminate();
	return 1;
}

// frame time 
void ShademEngine::calculateFrameTime() {
	float currentFrame = static_cast<float>(glfwGetTime());
	ShademEngine::deltaTime = currentFrame - ShademEngine::lastFrame;
	ShademEngine::lastFrame = currentFrame;
}

int ShademEngine::configure() {
	scene = new Scene();
	controller = new Controller::Controller(scene->getCamera());
	window = new Window();
	
	if (!window->configure()) {
		std::cout << "Failed to configure window" << std::endl;
		glfwTerminate();
		return -1;
	}
	
	if (!controller->configure(window->getGLFWwindow())) {
		std::cout << "Failed to configure controller" << std::endl;
		glfwTerminate();
		return -1;
	}

	scene->initBasicObjects();

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	return 0;
}