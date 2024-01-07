#pragma once

#include "Scene/Scene.h"

class RmLevel : public Scene {

public:
	RmLevel();

	int configure();
	void initBasicObjects();

	~RmLevel();

};