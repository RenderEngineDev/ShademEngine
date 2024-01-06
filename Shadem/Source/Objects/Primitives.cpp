#include "Objects/Primitives.h"

Primitives::Plane::Plane(ObjectAttributes::Common* attributes, Shader* shader) : Object(attributes) {
	this->shader = shader;
	setupMesh();
}

void Primitives::Plane::setupMesh() {

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

void Primitives::Plane::draw(Camera::Camera &camera) {
	this->shader->use();
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, attributes->position);
	model = glm::scale(model, attributes->scale);
	this->shader->setMat4("projection", camera.getProjection());
	this->shader->setMat4("view", camera.getView());
	this->shader->setMat4("model", model);

	mesh->Draw();
}

void Primitives::Plane::update(Camera::Camera& camera) {

}

Primitives::Cube::Cube(ObjectAttributes::Common* attributes, Shader* shader) : Object(attributes) {
	this->shader = shader;
	setupMesh();
}

void Primitives::Cube::setupMesh() {

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

void Primitives::Cube::draw(Camera::Camera &camera) {
	this->shader->use();
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, attributes->position);
	model = glm::scale(model, attributes->scale);
	this->shader->setMat4("projection", camera.getProjection());
	this->shader->setMat4("view", camera.getView());
	this->shader->setMat4("model", model);

	mesh->Draw();
}

void Primitives::Cube::update(Camera::Camera& camera) {

}

Primitives::RMSphere::RMSphere(ObjectAttributes::Sphereous* attributes, const char* fShaderPath) : RMObject(attributes, fShaderPath)
{

}

void Primitives::RMSphere::draw(Camera::Camera& camera)
{
	shader.use();
	/*
		definicje view przeniesc do kamery, aby mogla zwrocic sama rotacje bez przesuniecia
	*/
	glm::mat4 view = glm::mat4{glm::vec4(camera.right,0),
					 glm::vec4(camera.up,   0),
					 glm::vec4(camera.front,0),
					 glm::vec4(camera.right,1) };

	shader.setMat4("View", view);
	shader.setVec3("CameraPos", camera.position);
	shader.setVec3("SpherePos", attributes->position);
	shader.setFloat("Zoom", camera.zoom);
	shader.setVec2("WindowSize", camera.window->getWindowSize());
	shader.setVec3("Scale", attributes->scale);
	shader.setVec2("CameraRange", camera.getRange());

	mesh->Draw();
}

void Primitives::RMSphere::update(Camera::Camera& camera) {

}