#pragma once
#include "Objects/Object.h"
#include "GridGenerator.h"
#include "TrianglesGenerator.h"
#include "Camera/Camera.h"

#include <algorithm>
#include <random>

using namespace MarchingCubeGenerator;

namespace CubeMarching {
	class CmObject : public Object {

	public:

		CmObject(ObjectAttributes::CubeMarching* attributes, Shader *shader) : attributes(attributes), Object(attributes) {
			this->shader = shader;
		}

		virtual void draw(Camera::Camera& camera) = 0;
		virtual void update(Camera::Camera& camera) = 0;

		virtual ObjectAttributes::CubeMarching* getAttributes() const override {
			return attributes;
		}

	protected:
		ObjectAttributes::CubeMarching* attributes;
		TrianglesGenerator* trianglesGenerator = nullptr;
		GridGenerator* gridGenerator = nullptr;
	private:

		virtual void setupMesh() = 0;

	public:
		~CmObject() {
			delete trianglesGenerator;
		}
	};


	class Sphere : public CmObject {
	public:

		Sphere(ObjectAttributes::CubeMarching* attributes, Shader* shader);

		void draw(Camera::Camera& camera) override;
		void update(Camera::Camera& camera) override;

	private:

		void setupMesh() override;

		std::vector<Vertex> convertTrianglesToVertices(std::vector<std::vector<Point>> triangles);

		glm::vec3 transformToSphereSurface(glm::vec3& point, const bool& smoothing);

		~Sphere();
	};


	class Random : public CmObject {

	public:

		Random(ObjectAttributes::CubeMarching* attributes, Shader* shader);

		void draw(Camera::Camera& camera) override;
		void update(Camera::Camera& camera) override;

	private:

		void setupMesh() override;
		std::vector<Vertex> convertTrianglesToVertices(std::vector<std::vector<Point>> triangles);

	public:
		~Random();
	};


	class Noise : public CmObject {
	private:
		bool updated = false;
		glm::vec3 offset = glm::vec3(0.0f);
		std::vector<std::vector<std::vector<float>>> scalarFunction;

	public:
		Noise(ObjectAttributes::CubeMarching* attributes, Shader* shader);

		void draw(Camera::Camera& camera) override;
		void update(Camera::Camera& camera) override;

	private:

		void setupMesh() override;

		std::vector<Vertex> convertTrianglesToVertices(std::vector<std::vector<Point>> triangles);

	public:
		bool& isUpdated() { return updated; }
		void setUpdated(bool updated) { this->updated = updated; }
		/*float& getFrequency() { return frequency; }
		void setFrequency(float frequency) { this->frequency = frequency; }
		float& getAmplitude() { return amplitude; }
		void setAmplitude(float frequency) { this->amplitude = amplitude; }
		float& getLacunarity() { return lacunarity; }
		void setLacunarity(float frequency) { this->lacunarity = lacunarity; }
		float& getPersistence() { return persistence; }
		void setPersistence(float persistence) { this->persistence = persistence; }*/
		glm::vec3& getOffset() { return offset; }
		void setOffset(glm::vec3 offset) { this->offset = offset; }

		~Noise();
	};


}