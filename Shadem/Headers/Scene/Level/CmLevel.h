#pragma once

#include "Scene/Scene.h"

class CmLevel : public Scene {

public:
	CmLevel();

	int configure();
	void initBasicObjects();

	~CmLevel();

};