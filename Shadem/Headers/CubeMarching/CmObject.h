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

		CmObject(ObjectAttributes::CubeMarching::CommonCubeMarching* attributes, std::initializer_list<const std::string> filePaths) : attributes(attributes), Object(attributes) {
			auto shaderPair = ResourceManager::createOrGetShader(filePaths);
			this->shader = shaderPair.second;
			setShaderResourceKey(shaderPair.first);
		}

		//virtual void draw(Camera::Camera& camera) = 0;
		virtual void update(Camera::Camera& camera) = 0;

		virtual ObjectAttributes::CubeMarching::CommonCubeMarching* getAttributes() const override {
			return attributes;
		}

	protected:
		ObjectAttributes::CubeMarching::CommonCubeMarching* attributes = nullptr;
		TrianglesGenerator* trianglesGenerator = nullptr;
		GridGenerator* gridGenerator = nullptr;
	private:

		virtual void setupMesh() = 0;

	public:
		~CmObject() {
			delete trianglesGenerator;
			delete gridGenerator;
			delete attributes;
		}
	};


	class Sphere : public CmObject {
	public:
		ObjectAttributes::CubeMarching::Sphere* attributes = nullptr;

		Sphere(ObjectAttributes::CubeMarching::Sphere* attributes, std::initializer_list<const std::string> filePaths);

		virtual void draw(Camera::Camera& camera) override;
		virtual void update(Camera::Camera& camera) override;

		virtual ObjectAttributes::CubeMarching::Sphere* getAttributes() const override {
			return attributes;
		}

	private:
		void setupMesh() override;

		std::vector<Vertex> convertTrianglesToVertices(std::vector<std::vector<Point>> triangles);

		glm::vec3 transformToSphereSurface(glm::vec3& point, const bool& smoothing);
		glm::vec3 calculateFaceNormal(const std::vector<Point>& triangle);
		glm::vec3 calculateFaceNormal(const std::vector<glm::vec3>& triangle);
	};


	class Random : public CmObject {

	public:

		Random(ObjectAttributes::CubeMarching::CommonCubeMarching* attributes, std::initializer_list<const std::string> filePaths);

		//void draw(Camera::Camera& camera) override;
		virtual void update(Camera::Camera& camera) override;

	private:
		virtual void setupMesh() override;
		std::vector<Vertex> convertTrianglesToVertices(std::vector<std::vector<Point>> triangles);
	};

	class SPHSystem : public CmObject {
		public:

			float t = 0.01f;
			float ballScale = 0.01f;

			int n = 20;
			int N3 = n * n * n;
			unsigned int p, vx, vy, vz, rho, mass, fpressx, fpressy, fpressz, fviscx, fviscy, fviscz;

			const float GZ = -0.25f;
			int saveonoff = 0.0f;
			float mass0 = 0.00009f;

			std::vector<float> vx_cpu, vy_cpu, vz_cpu, rho_cpu, mass_cpu;
			std::vector<float> force_pres_x_cpu, force_pres_y_cpu, force_pres_z_cpu, force_visc_x_cpu, force_visc_y_cpu, force_visc_z_cpu;
			std::vector<glm::vec4> particles;

			//std::vector<Particle> particles;
			unsigned int cellValuesVBO;
			std::vector<GridCellValues> cellsValues;
			std::vector<std::vector<std::vector<float>>> scalarFunction;
			std::vector<float> scalarFunctionFlatted;
			ObjectAttributes::CubeMarching::Noise* attributes = nullptr;

			SPHSystem(int numParticles, glm::vec3 minBoundary, glm::vec3 maxBoundary, ObjectAttributes::CubeMarching::Noise* attributes, std::initializer_list<const std::string> filePaths);
			//~SPHSystem();
			//void update(float deltaTime);


			//GLuint particleBuffer;
			GLuint scalarFunctionBuffer;
			Shader* computeShader = nullptr;
			Shader* computeShaderForScalars = nullptr;
			Shader* computeShaderForNewScalars = nullptr;

			Shader* densityShader = nullptr;
			Shader* forcesShader = nullptr;
			Shader* particlesShader = nullptr;

			int numParticles;
			glm::vec3 minBoundary;
			glm::vec3 maxBoundary;
			const float particleRadius = 0.02f;
			const float kernelRadius = 5.0f;
		private:
			void uploadParticlesToGPU();
			void downloadParticlesFromGPU();
			void bindCellsValuesVBO();
			void setUniforms(Shader* shader);

			void SPHSystem::initGenerators();
			std::vector<Vertex> calculateCellsFirstVertices();

		public:
			//void draw(Camera::Camera& camera) override;
			virtual void draw(Camera::Camera& camera) override;
			virtual void update(Camera::Camera& camera) override;

		private:
			virtual void setupMesh() override;
	};


	class Noise : public CmObject {
	protected:
		bool updated = false;
		unsigned int cellValuesVBO;
		std::vector<GridCellValues> cellsValues;
		std::vector<std::vector<std::vector<float>>> scalarFunction;
		ObjectAttributes::CubeMarching::Noise* attributes = nullptr;

	public:
		Noise(ObjectAttributes::CubeMarching::Noise* attributes, std::initializer_list<const std::string> filePaths);

		virtual void draw(Camera::Camera& camera) override;
		virtual void update(Camera::Camera& camera) override;

		virtual ObjectAttributes::CubeMarching::Noise* getAttributes() const override {
			return attributes;
		}

	protected:
		virtual void setupMesh();

		std::vector<Vertex> convertTrianglesToVertices(std::vector<std::vector<Point>> triangles);
		glm::vec3 calculateFaceNormal(const std::vector<Point> &traingle);

		std::vector<Vertex> calculateCellsFirstVertices();
		void initGenerators();

	public:
		bool& isUpdated() { return updated; }
		void setUpdated(bool updated) { this->updated = updated; }

	};

	class Noise2D : public Noise {
	public:
		Noise2D(ObjectAttributes::CubeMarching::Noise* attributes, std::initializer_list<const std::string> filePaths);

		//virtual void draw(Camera::Camera& camera) override;
		virtual void update(Camera::Camera& camera) override;

	private:
		void setupMesh() override;
	};

	class Noise3D : public Noise {
	public:
		Noise3D(ObjectAttributes::CubeMarching::Noise* attributes, std::initializer_list<const std::string> filePaths);

		//virtual void draw(Camera::Camera& camera) override;
		virtual void update(Camera::Camera& camera) override;

	private:
		virtual void setupMesh() override;
	};

	class NoiseGeometry : public Noise {
	public:
		NoiseGeometry(ObjectAttributes::CubeMarching::Noise* attributes, std::initializer_list<const std::string> filePaths);

		virtual void draw(Camera::Camera& camera) override;
		virtual void update(Camera::Camera& camera) override;

		void bindCellsValuesVBO();

	private:
		virtual void setupMesh() = 0;
	};

	class NoiseGeometry2D : public NoiseGeometry {
	public:
		NoiseGeometry2D(ObjectAttributes::CubeMarching::Noise* attributes, std::initializer_list<const std::string> filePaths);
		
		virtual void update(Camera::Camera& camera) override;
	
	private:
		virtual void setupMesh() override;
	};

	class NoiseGeometry3D : public NoiseGeometry {
	public:
		NoiseGeometry3D(ObjectAttributes::CubeMarching::Noise* attributes, std::initializer_list<const std::string> filePaths);
		
		virtual void update(Camera::Camera& camera) override;
	
	private:
		virtual void setupMesh() override;
	};

	class Noise3DCompute : public Noise {
	public:

		unsigned int texture;
		Shader* rectangleShader = nullptr;
		Noise3DCompute(ObjectAttributes::CubeMarching::Noise* attributes, std::initializer_list<const std::string> filePaths);
		virtual void draw(Camera::Camera& camera) override;
		//virtual void update(Camera::Camera& camera) override;
	private:
		virtual void setupMesh() override;
	};
}