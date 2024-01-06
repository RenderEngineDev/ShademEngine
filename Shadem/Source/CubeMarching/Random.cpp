#pragma once

#include "CubeMarching/CmObject.h"

using namespace CubeMarching;

Random::Random(glm::vec3 gridSize, float isoValue, Shader* shader, glm::vec3 position, glm::vec3 scale) : CmObject(gridSize, isoValue, shader, position, scale) {
	setupMesh();
}

void Random::draw(Camera::Camera& camera) {
	shader->use();
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position);
	model = glm::scale(model, glm::vec3(4.0f * scale));
	shader->setMat4("projection", camera.getProjection());
	shader->setMat4("view", camera.getView());
	shader->setMat4("model", model);
	mesh->DrawWithoutIndices();
}

void Random::setupMesh() {
	marchingCubes = new MarchingCubes(gridSize);
	gridGenerator = new GridGenerator(gridSize);
	std::vector<std::vector<Point>> triangles = marchingCubes->triangulate_field(gridGenerator->generate_random_grid(), isoValue);
	std::vector<Vertex> vertices = convertTrianglesToVertices(triangles);
	mesh = new Mesh(vertices, std::vector<Texture>{});
}

std::vector<Vertex> Random::convertTrianglesToVertices(std::vector<std::vector<Point>> triangles) {
	std::vector<Vertex> vertices;
	for (auto triangle : triangles) {
		if (triangle.size() != 0)
			for (auto point : triangle) {
				Vertex vertex(glm::vec3(point.x, point.y, point.z), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f), glm::vec3(0.0f), glm::vec3(0.0f));
				vertices.push_back(vertex);
			}
	}
	return vertices;
}

Random::~Random() {

}
