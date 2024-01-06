#pragma once
#include "Objects/Object.h"
#include "GridGenerator.h"
#include "MarchingCube.h"
#include "Camera/Camera.h"

#include <algorithm>
#include <random>

using namespace MarchingCubeGenerator;

namespace CubeMarching {
	class CmObject : public Object {

	public:
		glm::vec3 gridSize;
		float isoValue;

		CmObject(glm::vec3 gridSize, float isoValue, Shader *shader, glm::vec3 position = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f)) : gridSize(gridSize), isoValue(isoValue), Object(position, scale) {
			this->shader = shader;
		}

		virtual void draw(Camera::Camera& camera) = 0;

	protected:
		MarchingCubes* marchingCubes = nullptr;
		GridGenerator* gridGenerator = nullptr;
	private:

		virtual void setupMesh() = 0;

	public:
		~CmObject() {
			delete marchingCubes;
		}
	};


	class Sphere : public CmObject {

	public:
		float radius;

		Sphere(glm::vec3 gridSize, float isoValue, Shader* shader, glm::vec3 position = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f), float radius = 1);

		void draw(Camera::Camera& camera) override;

	private:

		void setupMesh() override;

		std::vector<Vertex> convertTrianglesToVertices(std::vector<std::vector<Point>> triangles);

		glm::vec3 transformToSphereSurface(glm::vec3& point, const bool& smoothing);

	public:
		~Sphere();
	};


	class Random : public CmObject {

	public:

		Random(glm::vec3 gridSize, float isoValue, Shader* shader, glm::vec3 position = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f));

		void draw(Camera::Camera& camera) override;

	private:

		void setupMesh() override;
		std::vector<Vertex> convertTrianglesToVertices(std::vector<std::vector<Point>> triangles);

	public:
		~Random();
	};


	class Noise : public CmObject {

	public:

		Noise(glm::vec3 gridSize, float isoValue, Shader* shader, glm::vec3 position = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f));

		void draw(Camera::Camera& camera) override;

	private:

		void setupMesh() override;
		std::vector<Vertex> convertTrianglesToVertices(std::vector<std::vector<Point>> triangles);

	public:
		~Noise();
	};


}