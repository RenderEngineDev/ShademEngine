#pragma once

#include "CubeMarching/CmObject.h"

using namespace CubeMarching;

Noise::Noise(ObjectAttributes::CubeMarching* attributes, Shader* shader) : CmObject(attributes, shader) {
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
	mesh->DrawWithoutIndices();
}

void Noise::update(Camera::Camera& camera) {
	if (isUpdated()) {
		mesh->vertices = convertTrianglesToVertices(trianglesGenerator->triangulate_field(gridGenerator->generate_noise(getAttributes()->frequency, getAttributes()->amplitude, getAttributes()->lacunarity, getAttributes()->persistence, getAttributes()->noiseScale, offset, scalarFunction), attributes->isoValue));
		mesh->setupMeshWithouIndices();
		setUpdated(false);
	}
}

void Noise::setupMesh() {
	glm::vec3& noiseGridSize = attributes->gridSize;
	scalarFunction = std::vector<std::vector<std::vector<float>>>(noiseGridSize.x, std::vector<std::vector<float>>(noiseGridSize.y, std::vector<float>(noiseGridSize.z)));

	trianglesGenerator = new TrianglesGenerator(noiseGridSize);
	gridGenerator = new GridGenerator(noiseGridSize);
	std::vector<std::vector<Point>> triangles = trianglesGenerator->triangulate_field(gridGenerator->generate_noise(getAttributes()->frequency, getAttributes()->amplitude, getAttributes()->lacunarity, getAttributes()->persistence, getAttributes()->noiseScale, offset, scalarFunction), attributes->isoValue);
	std::vector<Vertex> vertices = convertTrianglesToVertices(triangles);
	mesh = new Mesh(vertices, std::vector<Texture>{});
}
//#include <chrono>

std::vector<Vertex> Noise::convertTrianglesToVertices(std::vector<std::vector<Point>> triangles) {
	//auto start = std::chrono::steady_clock::now();

	size_t totalSize = 0;
	for (const auto& vec : triangles) {
		totalSize += vec.size();
	}
	std::vector<Vertex> flatVector;
	flatVector.reserve(totalSize);
	for (const auto& vec : triangles) {
		for (const auto& vertex : vec) {
			flatVector.emplace_back(Vertex(glm::vec3(vertex.x, vertex.y, vertex.z)));
		}
	}
	//for (const auto& vec : triangles) {
		//flatVector.insert(flatVector.end(), vec.begin(), vec.end());
	//}

	//std::cout << "Elapsed(ms)= flat " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count() << std::endl;
	return flatVector;
}

Noise::~Noise() {

}
