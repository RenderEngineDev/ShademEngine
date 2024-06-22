#include "Objects/RMObject.h"


RMObject::RMObject() : Object(new ObjectAttributes::Common())
{
	auto meshPair = ResourceManager::createOrGetMesh("../Shadem/Assets/Default_Resources/Models/plane.obj");
	this->meshes = meshPair.second;
	this->setMeshResourceKey(meshPair.first);
}

RMObject::RMObject(ObjectAttributes::Common* attributes) : Object(attributes)
{
	auto meshPair = ResourceManager::createOrGetMesh("../Shadem/Assets/Default_Resources/Models/plane.obj");
	this->meshes = meshPair.second;
	this->setMeshResourceKey(meshPair.first);

	//setupMesh();
}

void RMObject::setUniforms(const Camera::Camera& camera)
{
	shader->setMat4("View", camera.getRotation());
	shader->setFloat("Zoom", camera.zoom);
	shader->setVec3("CameraPos", camera.position);
	shader->setVec2("CameraRange", camera.getRange());
	shader->setVec2("WindowSize", camera.window->getWindowSize());
	shader->setVec3("CameraFront", camera.front);
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

	std::vector<std::shared_ptr<Texture>> textures = std::vector<std::shared_ptr<Texture>>();

	this->meshes = std::make_shared<std::vector<Mesh*>>(std::vector<Mesh*>{new Mesh(vertices, indices, textures) });
}

