#include "Objects/Primitives.h"

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

Primitives::RMSphere::RMSphere(glm::vec3 position, glm::vec3 scale, const char* fShaderPath) : RMObject(position, scale, fShaderPath)
{

}

void Primitives::RMSphere::Draw(Camera &camera)
{
	shader.use();
	//glm::mat4 model = glm::mat4(1.0f);
	////model = glm::translate(model, position);
	//model = glm::scale(model, scale);
	//shader.setMat4("projection", projection);


	//glm::mat4 view = glm::mat4(1.0f);
	//view = glm::rotate(view, glm::radians(-camera.Pitch), glm::vec3(1.0f, 0.0f, 0.0f));
	//view = glm::rotate(view, glm::radians(camera.Yaw - 90), glm::vec3(0.0f, 1.0f, 0.0f));

	//glm::vec3 camPos = glm::vec3(camera.Position.x, camera.Position.y, camera.Position.z);
	////shader.setMat4("View", glm::lookAt(glm::vec3(0.0f), camera.Position + camera.Front, camera.Front));

	//float alpha = glm::dot(glm::vec3(0.0, 0.0, 1.0), camera.Front);

	//view = glm::mat4(1.0f);
	//view = glm::rotate(view, alpha, glm::cross(glm::vec3(0.0, 0.0, 1.0), camera.Front));

	glm::mat4 view = glm::mat4{glm::vec4(camera.Right,0),
					 glm::vec4(camera.Up,   0),
					 glm::vec4(camera.Front,0), 
					 glm::vec4(camera.Right,1)};
	//view = glm::lookAt(camera.Position, camera.Position + camera.Front, camera.Up);
	shader.setMat4("View", view);
	shader.setVec3("CameraPos", camera.Position);
	shader.setVec3("SpherePos", position);
	shader.setFloat("Zoom", camera.Zoom);
	shader.setMat4("Projection", glm::perspective(glm::radians(camera.Zoom), 1920.0f / 1080.0f, 0.1f, 250.0f));
	shader.setVec3("Forward", camera.Front);

	//std::cout << alpha << "\n";
	//std::cout << "x: " << camera.Front.x << " y: " << camera.Front.y << " z: " << camera.Front.z << "\n";
	//std::cout << "x: " << position.x << " y: " << position.y << " z: " << position.z << "\n";

	mesh->Draw(shader);
}
