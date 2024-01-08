#pragma once

#include "Renderer/CMRenderer.h"

CMRenderer::CMRenderer()
{
}

void CMRenderer::draw(Object* object, Camera::Camera* camera)
{
	object->draw(*camera);
}
