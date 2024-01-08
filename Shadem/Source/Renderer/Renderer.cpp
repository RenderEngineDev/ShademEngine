#pragma once

#include "Renderer/Renderer.h"

Renderer::Renderer()
{
	sRenderer = new StandardRenderer();
	rmRenderer = new RMRenderer();
	cmRenderer = new CMRenderer();
}

Renderer::~Renderer()
{
	delete sRenderer;
	delete rmRenderer;
	delete cmRenderer;
}

void Renderer::draw(Scene& scene)
{
	std::vector<Object*> objects = scene.getObjects();

	Camera::Camera* sceneCamera = scene.getCamera();

	for (auto& object : objects) {

		switch(object->getRenderType()) {
			case RenderType::RayMarchingRender:
				rmRenderer->draw(object, sceneCamera);
				break;
			
			case RenderType::CubeMarchingRender:
				cmRenderer->draw(object, sceneCamera);
				break;

			default:
				sRenderer->draw(object, sceneCamera);
				break;
		}
	}
}
