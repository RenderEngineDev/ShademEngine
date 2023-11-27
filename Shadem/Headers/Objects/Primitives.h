#pragma once

#include "Object.h"
#include "Shader/Shader.h"
#include <vector>
#include <glm/glm.hpp>
#include "Mesh.h"
#include "RMObject.h"
#include "Camera/Camera.h"

namespace Primitives {

	class Plane : public Object {
	public:
		Plane(glm::vec3 position, glm::vec3 scale);
		void Draw(Shader& shader, glm::mat4& projection, glm::mat4& view) override;
	private:
		void setupMesh();
	};

	class Cube : public Object {
	public:
		Cube(glm::vec3 position, glm::vec3 scale);
		void Draw(Shader& shader, glm::mat4& projection, glm::mat4& view) override;
	private:
		void setupMesh();
	};

	class RMSphere : public RMObject {
	public:
		RMSphere(glm::vec3 position, glm::vec3 scale, const char* fShaderPath);
		void Draw(Camera::Camera &camera);
	private:
	};

}