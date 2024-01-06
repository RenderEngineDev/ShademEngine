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
		Plane(ObjectAttributes::Common* attributes, Shader* shadere);
		virtual void draw(Camera::Camera &camera) override;
		virtual void update(Camera::Camera& camera) override;
	private:
		void setupMesh();
	};

	class Cube : public Object {
	public:
		Cube(ObjectAttributes::Common* attributes, Shader* shader);
		virtual void draw(Camera::Camera &camera) override;
		virtual void update(Camera::Camera& camera) override;
	private:
		void setupMesh();
	};

	class RMSphere : public RMObject {
	public:
		RMSphere(ObjectAttributes::Sphereous* attributes, const char* fShaderPath);
		virtual void draw(Camera::Camera& camera) override;
		virtual void update(Camera::Camera& camera) override;
	private:
		//void setupMesh();
	};

}