#pragma once

#include <chrono>

#include "Scene/Scene.h"
#include "Window/Controller.h"
#include "Window/Window.h"
#include "GUI/GUI.h"
#include "Renderer/Renderer.h"

class ShademEngine {

public:
	Scene* scene = nullptr;
	Controller::Controller* controller = nullptr;
	Window* window = nullptr;
	GUI* gui = nullptr;
	Renderer* renderer = nullptr;

	// timing
	static float lastFrame;
	static float deltaTime;
	//static float ttttime;
	//static std::chrono::time_point<std::chrono::steady_clock> start;

	ShademEngine();
	~ShademEngine();
	
	bool reloadScene(Scene *level);
	void checkAndReloadLevelSelection();

	int run();
	void calculateFrameTime();
	int configure();

	Scene* getScene() { return scene; }
};