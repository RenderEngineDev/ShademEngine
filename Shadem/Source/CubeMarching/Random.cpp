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
	size_t totalSize = 0;
	for (const auto& vec : triangles) {
		totalSize += vec.size();
	}
	std::vector<Vertex> flatVector;
	flatVector.reserve(totalSize);
	for (const auto& vec : triangles) {
		for (const auto& vertex : vec) {
			flatVector.push_back(Vertex(glm::vec3(vertex.x, vertex.y, vertex.z)));
		}
	}

	return flatVector;
}

Random::~Random() {

}
