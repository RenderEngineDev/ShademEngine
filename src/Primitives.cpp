#include "Primitives.h"

Primitives::Plane::Plane(glm::vec3 position = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f)) : Object(position, scale) {
	setupPrimitiveMesh();
}

void Primitives::Plane::setupPrimitiveMesh() {

	std::vector<Vertex> vertices = std::vector<Vertex>({
		{ glm::vec3( 1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f) },
		{ glm::vec3( 1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f) },
		{ glm::vec3(-1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f) },
		{ glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f) }
		});
	std::vector<unsigned int> indices = std::vector<unsigned int>({
			0, 1, 2,
			1, 2, 3,
		});
	std::vector<Texture> textures = std::vector<Texture>();

	this->mesh = new Mesh(vertices, indices, textures);
}

/*
	TODO: 
	przenieœæ projection i view do jakiegoœ zewnêtrznego renderera, który bêdzie ich referencje wykorzystywa³ przy rysowaniu wszystkich obiektów
*/
void Primitives::Plane::Draw(Shader& shader, glm::mat4 projection, glm::mat4 view) {
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position);
	model = glm::scale(model, scale);
	shader.setMat4("projection", projection);
	shader.setMat4("view", view);
	shader.setMat4("model", model);

	mesh->Draw(shader);
}

Primitives::Cube::Cube(glm::vec3 position = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f)) : Object(position, scale) {
	setupPrimitiveMesh();
}

void Primitives::Cube::setupPrimitiveMesh() {

	std::vector<Vertex> vertices = std::vector<Vertex>({
		{ glm::vec3(1.0f, -1.0f,  1.0f), glm::vec3(1.0f, -1.0f,  1.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.0f) },
		{ glm::vec3(1.0f,  1.0f,  1.0f), glm::vec3(1.0f,  1.0f,  1.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.0f) },
		{ glm::vec3(-1.0f,  1.0f,  1.0f), glm::vec3(-1.0f,  1.0f,  1.0f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.0f) },
		{ glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.0f) },
		{ glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(1.0f, -1.0f, -1.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.0f) },
		{ glm::vec3(1.0f,  1.0f, -1.0f), glm::vec3(1.0f,  1.0f, -1.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.0f) },
		{ glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.0f) },
		{ glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.0f) }
		});
	std::vector<unsigned int> indices = std::vector<unsigned int>({
		// front
		0, 1, 2,
		0, 2, 3,
		// right
		0, 1, 5,
		0, 4, 5,
		// left
		2, 3, 6,
		3, 6, 7,
		// top
		1, 2, 5,
		2, 5, 6,
		// bottom
		0, 3, 4,
		3, 4, 7,
		// back
		4, 5, 6,
		4, 6, 7
		});
	std::vector<Texture> textures = std::vector<Texture>();

	this->mesh = new Mesh(vertices, indices, textures);
}

/*
	TODO:
	przenieœæ projection i view do jakiegoœ zewnêtrznego renderera, który bêdzie ich referencje wykorzystywa³ przy rysowaniu wszystkich obiektów
*/
void Primitives::Cube::Draw(Shader& shader, glm::mat4 projection, glm::mat4 view) {
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position);
	model = glm::scale(model, scale);
	shader.setMat4("projection", projection);
	shader.setMat4("view", view);
	shader.setMat4("model", model);

	mesh->Draw(shader);
}