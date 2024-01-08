#pragma once

#include "Renderer/RMRenderer.h"

RMRenderer::RMRenderer()
{
}

void RMRenderer::draw(Object* object, Camera::Camera* camera)
{
	object->draw(*camera);
}
