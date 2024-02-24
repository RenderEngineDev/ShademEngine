#pragma once

#include "CubeMarching/CmObject.h"

using namespace CubeMarching;

Noise::Noise(ObjectAttributes::CubeMarching* attributes, const std::string& vertFilePath, const std::string& fragFilePath) : CmObject(attributes, vertFilePath, fragFilePath) {
	setupMesh();
}

void Noise::draw(Camera::Camera& camera) {
	shader->use();
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, attributes->position);
	model = glm::scale(model, glm::vec3(attributes->scale));
	shader->setMat4("projection", camera.getProjection());
	shader->setMat4("view", camera.getView());
	shader->setMat4("model", model);

	for (auto& mesh : *meshes) {
		mesh->DrawWithoutIndices(shader);
	}
}

void Noise::update(Camera::Camera& camera) {
	if (isUpdated()) {
		(*meshes)[0]->vertices = convertTrianglesToVertices(trianglesGenerator->triangulate_field(gridGenerator->generate_noise(getAttributes()->frequency, getAttributes()->amplitude, getAttributes()->lacunarity, getAttributes()->persistence, offset, scalarFunction), attributes->isoValue));
		(*meshes)[0]->setupMeshWithouIndices();
		setUpdated(false);
	}
}

void Noise::setupMesh() {
	glm::vec3& noiseGridSize = attributes->gridSize;
	scalarFunction = std::vector<std::vector<std::vector<float>>>(noiseGridSize.x, std::vector<std::vector<float>>(noiseGridSize.y, std::vector<float>(noiseGridSize.z)));

	trianglesGenerator = new TrianglesGenerator(noiseGridSize);
	gridGenerator = new GridGenerator(noiseGridSize);
	std::vector<std::vector<Point>> triangles = trianglesGenerator->triangulate_field(gridGenerator->generate_noise(getAttributes()->frequency, getAttributes()->amplitude, getAttributes()->lacunarity, getAttributes()->persistence, offset, scalarFunction), attributes->isoValue);
	std::vector<Vertex> vertices = convertTrianglesToVertices(triangles);
	meshes = std::make_shared<std::vector<Mesh*>>(std::vector<Mesh*>{new Mesh(vertices, std::vector<std::shared_ptr<Texture>>{})});
}

std::vector<Vertex> Noise::convertTrianglesToVertices(std::vector<std::vector<Point>> triangles) {
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

Noise::~Noise() {

}
