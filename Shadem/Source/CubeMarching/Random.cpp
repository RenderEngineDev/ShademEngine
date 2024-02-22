#pragma once

#include "CubeMarching/CmObject.h"

using namespace CubeMarching;

Random::Random(ObjectAttributes::CubeMarching::CommonCubeMarching* attributes, std::initializer_list<const std::string> filePaths) : CmObject(attributes, filePaths) {
	setupMesh();
}

//void Random::draw(Camera::Camera& camera) {
//}

void Random::update(Camera::Camera& camera) {
}

void Random::setupMesh() {
	trianglesGenerator = new TrianglesGenerator(attributes->gridSize);
	gridGenerator = new GridGenerator(attributes->gridSize);
	trianglesGenerator->triangulate_field(gridGenerator->generate_random_grid(), attributes->isoValue);
	std::vector<Vertex> vertices = convertTrianglesToVertices(trianglesGenerator->triangles);
	meshes = std::make_shared<std::vector<Mesh*>>(std::vector<Mesh*>{new Mesh(vertices, std::vector<std::shared_ptr<Texture>>{})});
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