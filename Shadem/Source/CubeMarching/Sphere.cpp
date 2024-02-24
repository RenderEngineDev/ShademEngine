#pragma once

#include "CubeMarching/CmObject.h"

using namespace CubeMarching;

Sphere::Sphere(ObjectAttributes::CubeMarching* attributes, Shader* shader) : CmObject(attributes, shader) {
	setupMesh();
}

// skala mno¿ona x4.0, poniewa¿ pozycje wierzcho³ków sfery s¹ tworzone dla promienia 1/4
void Sphere::draw(Camera::Camera& camera) {
	shader->use();
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, attributes->position);
	model = glm::scale(model, glm::vec3(4.0f * getAttributes()->radius * attributes->scale));
	shader->setMat4("projection", camera.getProjection());
	shader->setMat4("view", camera.getView());
	shader->setMat4("model", model);
	mesh->DrawWithoutIndices();
}

void Sphere::update(Camera::Camera& camera) {

}

void Sphere::setupMesh() {
	trianglesGenerator = new TrianglesGenerator(attributes->gridSize);
	gridGenerator = new GridGenerator(attributes->gridSize);
	trianglesGenerator->triangulate_field(gridGenerator->generate_sphere(), attributes->isoValue);
	std::vector<Vertex> vertices = convertTrianglesToVertices(trianglesGenerator->triangles);
	mesh = new Mesh(vertices, std::vector<Texture>{});
}

std::vector<Vertex> Sphere::convertTrianglesToVertices(std::vector<std::vector<Point>> triangles) {
	size_t totalSize = 0;
	for (const auto& vec : triangles) {
		totalSize += vec.size();
	}
	std::vector<Vertex> flatVector;
	flatVector.reserve(totalSize);
	for (const auto& vec : triangles) {
		for (const auto& vertex : vec) {
			flatVector.push_back(Vertex(transformToSphereSurface(glm::vec3(vertex.x, vertex.y, vertex.z), false)));
		}
	}
	return flatVector;
}

glm::vec3 Sphere::transformToSphereSurface(glm::vec3& point, const bool& smoothing) {
	if (!smoothing) return point;

	glm::vec3 origin = glm::vec3(0.0f);
	// Oblicz wektor od sfery do punktu
	glm::vec3 vectorToPoint = point - origin;

	// SprawdŸ, czy punkt jest ju¿ na powierzchni sfery
	float distanceToSurface = glm::length(vectorToPoint);
	if (distanceToSurface == getAttributes()->radius) {
		return point;
	}
	// TODO: to 0.25f to pierwotny promieñ generowanych punktów w generatorze - z oboma trzeba bêdzie coœ zrobiæ
	return origin + glm::normalize(vectorToPoint) * 0.25f;
}

Sphere::~Sphere() {

}
