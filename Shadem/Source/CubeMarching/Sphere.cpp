#pragma once

#include "CubeMarching/CmObject.h"

using namespace CubeMarching;

Sphere::Sphere(glm::vec3 gridSize, float isoValue, Shader* shader, glm::vec3 position, glm::vec3 scale, float radius) : radius(radius), CmObject(gridSize, isoValue, shader, position, scale) {
	setupMesh();
}

void Sphere::draw(Camera::Camera& camera) {
	shader->use();
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position);
	model = glm::scale(model, glm::vec3(4.0f * radius * scale));
	shader->setMat4("projection", camera.getProjection());
	shader->setMat4("view", camera.getView());
	shader->setMat4("model", model);
	mesh->DrawWithoutIndices();
}

void Sphere::setupMesh() {
	marchingCubes = new MarchingCubes(gridSize);
	gridGenerator = new GridGenerator(gridSize);
	std::vector<std::vector<Point>> triangles = marchingCubes->triangulate_field(gridGenerator->generate_sphere(), isoValue);
	std::vector<Vertex> vertices = convertTrianglesToVertices(triangles);
	mesh = new Mesh(vertices, std::vector<Texture>{});
}

std::vector<Vertex> Sphere::convertTrianglesToVertices(std::vector<std::vector<Point>> triangles) {
	std::vector<Vertex> vertices;
	for (auto triangle : triangles) {
		if (triangle.size() != 0)
			for (auto point : triangle) {
				Vertex vertex(transformToSphereSurface(glm::vec3(point.x, point.y, point.z), false), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f), glm::vec3(0.0f), glm::vec3(0.0f));
				vertices.push_back(vertex);
			}
	}
	return vertices;
}

glm::vec3 Sphere::transformToSphereSurface(glm::vec3& point, const bool& smoothing) {
	if (!smoothing) return point;

	glm::vec3 origin = glm::vec3(0.0f);
	// Oblicz wektor od sfery do punktu
	glm::vec3 vectorToPoint = point - origin;

	// SprawdŸ, czy punkt jest ju¿ na powierzchni sfery
	float distanceToSurface = glm::length(vectorToPoint);
	if (distanceToSurface == radius) {
		// Punkt ju¿ znajduje siê na powierzchni sfery, nie trzeba go transformowaæ
		return point;
	}

	// Normalizuj wektor, aby przekszta³ciæ go na powierzchniê sfery
	// Przemnó¿ wektor przez promieñ sfery, aby uzyskaæ punkt na powierzchni sfery
	// TODO: to 0.25f to pierwotny promieñ generowanych punktów w generatorze - oba trzeba bêdzie wrzuciæ do obiektu cmSphere jako zmienna
	return origin + glm::normalize(vectorToPoint) * 0.25f;
}

Sphere::~Sphere() {

}
