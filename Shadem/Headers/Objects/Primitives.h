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
		Plane(Shader* shader, glm::vec3 position, glm::vec3 scale);
		virtual void draw(Camera::Camera &camera) override;
	private:
		void setupMesh();
	};

	class Cube : public Object {
	public:
		Cube(Shader* shader, glm::vec3 position, glm::vec3 scale);
		virtual void draw(Camera::Camera &camera) override;
	private:
		void setupMesh();
	};

	class RMSphere : public RMObject {
	public:
		RMSphere(glm::vec3 position, glm::vec3 scale, const char* fShaderPath);
		virtual void draw(Camera::Camera& camera) override;
	private:
		//void setupMesh();
	};

}