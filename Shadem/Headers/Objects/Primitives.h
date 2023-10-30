#pragma once

#include "Object.h"
#include "Shader/Shader.h"
#include <vector>
#include <glm/glm.hpp>
#include "Mesh.h"

namespace Primitives {

	class Plane : public Object {
	public:
		Plane(glm::vec3 position, glm::vec3 scale);
		void Draw(Shader& shader, glm::mat4 projection, glm::mat4 view);
	private:
		virtual void setupPrimitiveMesh();
	};

	class Cube : public Object {
	public:
		Cube(glm::vec3 position, glm::vec3 scale);
		void Draw(Shader& shader, glm::mat4 projection, glm::mat4 view);
	private:
		virtual void setupPrimitiveMesh();
	};

}