#pragma once

#include "Renderer/StandardRenderer.h"

StandardRenderer::StandardRenderer()
{
}

void StandardRenderer::draw(Object* object, Camera::Camera* camera)
{
	object->draw(*camera);
}
