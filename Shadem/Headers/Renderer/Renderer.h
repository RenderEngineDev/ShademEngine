#pragma once
#include "Scene/Scene.h"
#include "StandardRenderer.h"
#include "RMRenderer.h"
#include "CMRenderer.h"


class Renderer
{
private:

	StandardRenderer* sRenderer = nullptr;
	RMRenderer* rmRenderer = nullptr;
	CMRenderer* cmRenderer = nullptr;


public:

	Renderer();
	~Renderer();

	void draw(Scene& scene);

};
