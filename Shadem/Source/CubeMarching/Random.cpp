#pragma once

#include "CubeMarching/CmObject.h"

using namespace CubeMarching;

Random::Random(ObjectAttributes::CubeMarching* attributes, Shader* shader) : CmObject(attributes, shader) {
	setupMesh();
}

void Random::draw(Camera::Camera& camera) {
	shader->use();
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, attributes->position);
	model = glm::scale(model, glm::vec3(1.0f * attributes->scale));
	shader->setMat4("projection", camera.getProjection());
	shader->setMat4("view", camera.getView());
	shader->setMat4("model", model);
	mesh->DrawWithoutIndices();
}

void Random::update(Camera::Camera& camera) {

}

void Random::setupMesh() {
	trianglesGenerator = new TrianglesGenerator(attributes->gridSize);
	gridGenerator = new GridGenerator(attributes->gridSize);
	std::vector<std::vector<Point>> triangles = trianglesGenerator->triangulate_field(gridGenerator->generate_random_grid(), attributes->isoValue);
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
