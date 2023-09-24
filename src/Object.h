#pragma once

#include "Mesh.h"

class Object {

protected:
	glm::vec3 position;
	glm::vec3 scale;
	Mesh* mesh = nullptr;

	/*
		// Do przysz³ej edycji obiektów
		glm::vec3 Front;
		glm::vec3 Up;
		glm::vec3 Right;
	*/


private:
	virtual void setupPrimitiveMesh() = 0;

public:
	Object(glm::vec3 position, glm::vec3 scale);
	~Object() {
		delete mesh;
	}

};