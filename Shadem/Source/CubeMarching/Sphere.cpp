#pragma once

#include "CubeMarching/CmObject.h"
#include "Objects/Lights/Light.h"

using namespace CubeMarching;

Sphere::Sphere(ObjectAttributes::CubeMarching::Sphere* attributes, std::initializer_list<const std::string> filePaths) : attributes(attributes), CmObject(attributes, filePaths) {
	setupMesh();
}

// skala mno¿ona x4.0, poniewa¿ pozycje wierzcho³ków sfery s¹ tworzone dla promienia 1/4
void Sphere::draw(Camera::Camera& camera) {
	shader->use();
	glm::mat4 model = glm::mat4(1.0f);
	evaluateBasicModelTransformations(model);
	model = glm::scale(model, glm::vec3(4.0f * getAttributes()->radius));
	shader->setMat4("projection", camera.getProjection());
	shader->setMat4("view", camera.getView());
	shader->setMat4("model", model);
	shader->setMat4("MVP", camera.getProjection() * camera.getView() * model);
	shader->setVec3("cameraPosition", camera.position);

	Lights::Light::setShaderLights(shader.get());

	for (auto& mesh : (*meshes))
		mesh->DrawWithoutIndices(shader);
}

void Sphere::update(Camera::Camera& camera) {
}

void Sphere::setupMesh() {
	trianglesGenerator = new TrianglesGenerator(attributes->gridSize);
	gridGenerator = new GridGenerator(attributes->gridSize);
	trianglesGenerator->triangulate_field(gridGenerator->generate_sphere(), attributes->isoValue);
	std::vector<Vertex> vertices = convertTrianglesToVertices(trianglesGenerator->triangles);
	meshes = std::make_shared<std::vector<Mesh*>>(std::vector<Mesh*>{new Mesh(vertices, std::vector<std::shared_ptr<Texture>>{})});
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
			flatVector.push_back(Vertex(transformToSphereSurface(glm::vec3(vertex.x, vertex.y, vertex.z), false), calculateFaceNormal(vec)));
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

glm::vec3 Sphere::calculateFaceNormal(const std::vector<Point>& triangle) {
	return glm::normalize(glm::cross(glm::vec3(triangle[1].x - triangle[0].x, triangle[1].y - triangle[0].y, triangle[1].z - triangle[0].z),
		glm::vec3(triangle[2].x - triangle[0].x, triangle[2].y - triangle[0].y, triangle[2].z - triangle[0].z)));
}