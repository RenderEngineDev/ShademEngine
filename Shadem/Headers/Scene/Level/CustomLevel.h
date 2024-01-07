#pragma once

#include "Scene/Scene.h"

class CustomLevel : public Scene {

public:
	CustomLevel();

	int configure();
	void initBasicObjects();

	~CustomLevel();

};