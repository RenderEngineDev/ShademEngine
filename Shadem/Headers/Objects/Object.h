#pragma once

#include "Objects/Mesh.h"
#include "Shader/Shader.h"


struct ObjectBasicAttributes {
	float radius = 1;
	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 scale = glm::vec3(1.0f);
	glm::vec3 rotation = glm::vec3(0.0f);

	ObjectBasicAttributes(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation, float radius) : position(position), scale(scale), rotation(rotation), radius(radius) {};
	ObjectBasicAttributes(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation) : position(position), scale(scale), rotation(rotation) {};
	ObjectBasicAttributes(glm::vec3 position, glm::vec3 scale) : position(position), scale(scale) {};
	ObjectBasicAttributes(glm::vec3 position) : position(position) {};
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


private:
	virtual void setupMesh() = 0;

public:
	Object() {};
	Object(glm::vec3 position, glm::vec3 scale);
	virtual void Draw(Shader& shader, glm::mat4 &projection, glm::mat4 &view) = 0;
	~Object() {
		delete mesh;
		delete shader;
	}

};