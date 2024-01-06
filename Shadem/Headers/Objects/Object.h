#pragma once

#include "Objects/Mesh.h"
#include "Objects/Attributes.h"
#include "Shader/Shader.h"
#include "Camera/Camera.h"

enum RenderType
{
	StandardRender,
	RayMarchingRender,
	CubeMarchingRender
};
class Object {

protected:
	ObjectAttributes::Common* attributes;

	// float yaw;
	// float pitch;

	// glm::vec3 front;
	// glm::vec3 up;
	// glm::vec3 right;
	// glm::vec3 worldUp;

	Shader* shader = nullptr;
	Mesh* mesh = nullptr;
	RenderType renderer = RenderType::StandardRender;

private:
	virtual void setupMesh() = 0;

public:
	Object() {};
	Object(ObjectAttributes::Common* attributes);
	virtual void draw(Camera::Camera &camera) = 0;
	virtual void update(Camera::Camera& camera) = 0;

	virtual ObjectAttributes::Common* getAttributes() const { 
		return attributes; 
	}

	~Object() {
		delete mesh;
		delete shader;
		delete attributes;
	}

	RenderType& getRenderType() { return renderer; }

};