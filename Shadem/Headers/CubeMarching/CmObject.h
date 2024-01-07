#pragma once
#include "Objects/Object.h"
#include "Generator.h"
#include "MarchingCube.h"
#include "Camera/Camera.h"


class CmObject : public Object {

public:
	int sizeOfGrid;
	float radius;
	float isoValue;
	MarchingCubeGenerator::StructureType structureType;
	glm::vec3 position;

	CmObject(MarchingCubeGenerator::StructureType structureType, int sizeOfGrid, float isoValue, Shader *shader, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), float radius = 1) : structureType(structureType), sizeOfGrid(sizeOfGrid), position(position), radius(radius), isoValue(isoValue) {
		this->shader = shader;
		setupMesh();
	}

	void draw(Camera::Camera &camera) override {
		this->shader->use();
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1.0f));
		model = glm::scale(model, glm::vec3(80.0f));
		//model = glm::translate(model, position);
		//model = glm::scale(model, scale);
		this->shader->setMat4("projection", camera.getProjection());
		this->shader->setMat4("view", camera.getView());
		this->shader->setMat4("model", model);
		this->mesh->DrawWithoutIndices();
	}

private :
	MarchingCubes* marchingCubes = nullptr;
	
	// Odziedziczono za poœrednictwem elementu Object
	void setupMesh() override {
		marchingCubes = new MarchingCubes(sizeOfGrid);
		MarchingCubeGenerator::Generator generator(sizeOfGrid);
		std::vector<std::vector<Point>> triangles = marchingCubes->triangulate_field(getStructureGrid(generator), isoValue);
		std::vector<Vertex> vertices = convertTrianglesToVertices(triangles);
		this->mesh = new Mesh(vertices, std::vector<Texture>{});

	}
	
	std::vector<Vertex> convertTrianglesToVertices(std::vector<std::vector<Point>> triangles) {
		std::vector<Vertex> vertices;
		for (auto triangle : triangles) {
			for (auto point : triangle) {
				Vertex vertex(glm::vec3(point.x, point.y, point.z), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f), glm::vec3(0.0f), glm::vec3(0.0f));
				vertices.push_back(vertex);
			}
		}
		return vertices;
	}

	std::vector<std::vector<std::vector<float>>> getStructureGrid( MarchingCubeGenerator::Generator &generator) {
		switch (structureType)
		{
			case MarchingCubeGenerator::RANDOM:
				return generator.generate_random_grid();
			case MarchingCubeGenerator::SPHERE:
				return generator.generate_sphere(position, radius);
			case MarchingCubeGenerator::NESTED_SPHERES:
				return generator.generate_nested_spheres();
			default:
				return std::vector<std::vector<std::vector<float>>>{};
		}
	}

public:
	~CmObject() {
		delete marchingCubes;
	}
};
