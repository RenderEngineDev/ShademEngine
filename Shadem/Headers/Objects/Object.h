#pragma once

#include "Objects/Mesh.h"
#include "Shader/Shader.h"
#include "Camera/Camera.h"

enum RenderType
{
	StandardRender,
	RayMarchingRender,
	CubeMarchingRender
};

// czysto abstrakcyjna klasa, na potrzeby dynamicznego castowania (patrz np. CubeMarchingObjectFactory.cpp) - oraz do przysz³ych zmian na³o¿enia template'a na metode createObject (ujednolicenie metody)
struct ObjectAttributes {
	virtual void doNothing() = 0;
};

struct ObjectBasicAttributes : public ObjectAttributes {
	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 scale = glm::vec3(1.0f);
	glm::vec3 rotation = glm::vec3(0.0f);

	void doNothing() override {};

	ObjectBasicAttributes(glm::vec3 position = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f), glm::vec3 rotation = glm::vec3(0.0f)) : position(position), scale(scale), rotation(rotation) {};
};

struct SphereousAttributes : public ObjectBasicAttributes {
	float radius = 1;

	void doNothing() override {};

	SphereousAttributes(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation, float radius) : radius(radius), ObjectBasicAttributes(position, scale, rotation) {};
	SphereousAttributes(glm::vec3 position = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f), glm::vec3 rotation = glm::vec3(0.0f)) : ObjectBasicAttributes(position, scale, rotation) {};
};

struct CubeMarchingAttributes : public ObjectBasicAttributes {
	float radius = 1;
	float isoValue = 1.0f;
	glm::vec3 gridSize = glm::vec3(20.0f);

	void doNothing() override {};

	CubeMarchingAttributes(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation, glm::vec3 gridSize, float isoValue, float radius) : gridSize(gridSize), isoValue(isoValue), radius(radius), ObjectBasicAttributes(position, scale, rotation) {};
	CubeMarchingAttributes(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation, glm::vec3 gridSize, float isoValue) : gridSize(gridSize), isoValue(isoValue), ObjectBasicAttributes(position, scale, rotation) {};
	CubeMarchingAttributes(glm::vec3 position = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f), glm::vec3 rotation = glm::vec3(0.0f)) : ObjectBasicAttributes(position, scale, rotation) {};
};

class Object {

protected:
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation;

	// TODO: zamieniæ wszystkie wywo³ania powy¿szych pól na wykorzystanie poni¿szego attribute
	// ObjectBasicAttributes attribute;

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
	Object(glm::vec3 position, glm::vec3 scale);
	virtual void draw(Camera::Camera &camera) = 0;
	~Object() {
		delete mesh;
		delete shader;
	}

	RenderType& getRenderType() { return renderer; }

};