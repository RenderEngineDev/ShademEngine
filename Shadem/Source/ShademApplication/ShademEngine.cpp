#pragma once

#include "ShademApplication/ShademEngine.h"

#include <iostream>
#include "Camera/Camera.h"
#include "Shader/Shader.h"
#include "Objects/Primitives.h"
#include "CubeMarching/CmObject.h"
#include "Scene/Level/CmLevel.h"
#include "Scene/Level/CustomLevel.h"
#include "Scene/Level/RmLevel.h"

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

	while (!glfwWindowShouldClose(window->getGLFWwindow())) {
		glClearColor(0.2f, 0.3f, 0.7f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		calculateFrameTime();
		controller->processInput(window->getGLFWwindow());

		checkAndReloadLevelSelection();
		if (scene) { 
			scene->getCamera()->processViewAndProjection();
			scene->draw();
		}

		gui->startGuiFrame();
		gui->draw();
		
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

// TODO: W przysz³oœci zmieniæ na generyczne zaci¹ganie nazw zapisanych poziomów do wyboru z pliku
void ShademEngine::checkAndReloadLevelSelection() {
	if (gui->listboxItemCurrent != -1) {
		switch (gui->listboxItemCurrent) {
		case 0: {gui->listboxItemCurrent = -1;	 reloadScene(new CmLevel());	 break; };
		case 1: {gui->listboxItemCurrent = -1;	 reloadScene(new RmLevel());	 break; };
		case 2: {gui->listboxItemCurrent = -1;	 reloadScene(new CustomLevel()); break; };
		default: {gui->listboxItemCurrent = -1;  reloadScene(new CustomLevel()); break; };
		}
	}
}

bool ShademEngine::reloadScene(Scene *level) {
	if (level) {
		delete scene;
		scene = level;
		scene->initBasicObjects();
		controller->setCamera(scene->getCamera());
		return true;
	}
	return false;
}

int ShademEngine::configure() {
	controller = new Controller::Controller();
	window = new Window();
	gui = new GUI();
	
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

	if (!gui->configure(window->getGLFWwindow())) {
		std::cout << "Failed to configure GUI" << std::endl;
		glfwTerminate();
		return -1;
	}
	gui->setupMenuProperties();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	return 0;
}