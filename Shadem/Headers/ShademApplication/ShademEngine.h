#pragma once

#include "Scene/Scene.h"
#include "Window/Controller.h"
#include "Window/Window.h"
#include "GUI/GUI.h"

class ShademEngine {

public:
	Scene* scene = nullptr;
	Controller::Controller* controller = nullptr;
	Window* window = nullptr;
	GUI* gui = nullptr;

	// timing
	static float lastFrame;
	static float deltaTime;

	ShademEngine();
	
	bool reloadScene(Scene *level);
	void checkAndReloadLevelSelection();

	int run();
	void calculateFrameTime();
	int configure();
};