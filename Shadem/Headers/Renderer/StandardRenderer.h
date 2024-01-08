#pragma once

#include "Objects/Object.h"
#include "Scene/Scene.h"

class StandardRenderer
{
private:



public:

	StandardRenderer();

	void draw(Object* object, Camera::Camera* camera);

};