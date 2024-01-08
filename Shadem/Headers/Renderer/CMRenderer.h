#pragma once

#include "Objects/Object.h"
#include "Scene/Scene.h"

class CMRenderer
{
private:



public:

	CMRenderer();

	void draw(Object* obj, Camera::Camera* camera);

};
