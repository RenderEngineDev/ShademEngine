#include "Objects/RMObject.h"


RMObject::RMObject(ObjectAttributes::Common* attributes, const char* fShaderPath) : Object(attributes), shader("../Shadem/Shaders/RMShaders/Vshader.shader", fShaderPath)
{
	setupMesh();
}

void RMObject::setupMesh()
{
	std::vector<Vertex> vertices = {
	{ glm::vec3(-1.0f,  1.0f, 0.0f), glm::vec3(1.0f, -1.0f,  1.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.0f) },
	{ glm::vec3(1.0f,  1.0f, 0.0f), glm::vec3(1.0f,  1.0f,  1.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.0f) },
	{ glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(-1.0f,  1.0f,  1.0f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.0f) },
	{ glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.0f) }
		};

	std::vector<unsigned int> indices = {
		 0, 2, 1,
		 1, 2, 3
	};

	std::vector<Texture> textures = std::vector<Texture>();

	this->mesh = new Mesh(vertices, indices, textures);
}

