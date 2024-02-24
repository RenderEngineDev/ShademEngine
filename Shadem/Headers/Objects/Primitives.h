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
		Plane(ObjectAttributes::Common* attributes, const std::string& vertFilePath, const std::string& fragFilePath);
		//virtual void draw(Camera::Camera &camera) override;
		virtual void update(Camera::Camera& camera) override;
	private:
		void setupMesh();
	};

	class Cube : public Object {
	public:
		Cube(ObjectAttributes::Common* attributes, const std::string& vertFilePath, const std::string& fragFilePath);
		//virtual void draw(Camera::Camera &camera) override;
		virtual void update(Camera::Camera& camera) override;
	private:
		void setupMesh();
	};

	class Cone : public Object {
	public:
		Cone(ObjectAttributes::Common* attributes, const std::string& vertFilePath, const std::string& fragFilePath);
		//virtual void draw(Camera::Camera& camera) override;
		virtual void update(Camera::Camera& camera) override;
	private:
		void setupMesh();
	};

	class Torus : public Object {
	public:
		Torus(ObjectAttributes::Common* attributes, const std::string& vertFilePath, const std::string& fragFilePath);
		//virtual void draw(Camera::Camera& camera) override;
		virtual void update(Camera::Camera& camera) override;
	private:
		void setupMesh();
	};

	class Cylinder : public Object {
	public:
		Cylinder(ObjectAttributes::Common* attributes, const std::string& vertFilePath, const std::string& fragFilePath);
		//virtual void draw(Camera::Camera& camera) override;
		virtual void update(Camera::Camera& camera) override;
	private:
		void setupMesh();
	};

	class Sphere : public Object {
	public:
		Sphere(ObjectAttributes::Common* attributes, const std::string& vertFilePath, const std::string& fragFilePath);
		//virtual void draw(Camera::Camera& camera) override;
		virtual void update(Camera::Camera& camera) override;
	private:
		void setupMesh();
	};

	class Circle : public Object {
	public:
		Circle(ObjectAttributes::Common* attributes, const std::string& vertFilePath, const std::string& fragFilePath);
		//virtual void draw(Camera::Camera& camera) override;
		virtual void update(Camera::Camera& camera) override;
	private:
		void setupMesh();
	};

	class RMSphere : public RMObject {
	public:
		RMSphere(ObjectAttributes::Sphereous* attributes, const std::string& fragFilePath);
		virtual void draw(Camera::Camera& camera) override;
		virtual void update(Camera::Camera& camera) override;
	private:
		//void setupMesh();
	};

}