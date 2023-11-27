#pragma once

#include "Scene/Scene.h"
#include "Window/Controller.h"
#include "Window/Window.h"


class ShademEngine {
	// CubeMarchingRenderer* cubeMarchingRenderer;

public:
	Scene* scene = nullptr;
	Controller::Controller* controller = nullptr;
	Window* window = nullptr;

	// timing
	static float lastFrame;
	static float deltaTime;

	ShademEngine();

	int run();
	void calculateFrameTime();
	int configure();
};