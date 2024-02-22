#include "Objects/Primitives.h"

Primitives::Plane::Plane(ObjectAttributes::Common* attributes, const std::string& vertFilePath, const std::string& fragFilePath) : Object(attributes) {
	auto shaderPair = ResourceManager::createOrGetShader({vertFilePath, fragFilePath});
	this->shader = shaderPair.second;
	this->setShaderResourceKey(shaderPair.first);

	auto meshPair = ResourceManager::createOrGetMesh("../Shadem/Assets/Default_Resources/Models/plane.obj");
	this->meshes = meshPair.second;
	this->setMeshResourceKey(meshPair.first);

	//setupMesh();
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
	std::vector<std::shared_ptr<Texture>> textures = std::vector<std::shared_ptr<Texture>>();

	this->meshes = std::make_shared<std::vector<Mesh*>>(std::vector<Mesh*>{new Mesh(vertices, indices, textures) });
}

void Primitives::Plane::update(Camera::Camera& camera) {

}

Primitives::Cube::Cube(ObjectAttributes::Common* attributes, const std::string& vertFilePath, const std::string& fragFilePath) : Object(attributes) {
	auto shaderPair = ResourceManager::createOrGetShader({vertFilePath, fragFilePath});
	this->shader = shaderPair.second;
	this->setShaderResourceKey(shaderPair.first);

	auto meshPair = ResourceManager::createOrGetMesh("../Shadem/Assets/Default_Resources/Models/cube.obj");
	this->meshes = meshPair.second;
	this->setMeshResourceKey(meshPair.first);

	//setupMesh();
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
	std::vector<std::shared_ptr<Texture>> textures = std::vector<std::shared_ptr<Texture>>();

	this->meshes = std::make_shared<std::vector<Mesh*>>(std::vector<Mesh*>{new Mesh(vertices, indices, textures)});
}

void Primitives::Cube::update(Camera::Camera& camera) {

}

Primitives::RMSphere::RMSphere(ObjectAttributes::Sphereous* attributes, const std::string& fragFilePath) : RMObject(attributes)
{
	auto shaderPair = ResourceManager::createOrGetShader({Shaders::getRmVertexShaderPath(), fragFilePath});
	this->shader = shaderPair.second;
	this->setShaderResourceKey(shaderPair.first);
}

void Primitives::RMSphere::draw(Camera::Camera& camera)
{
	shader->use();
	
	shader->setMat4("View", camera.getRotation());
	shader->setFloat("Zoom", camera.zoom);
	shader->setVec3("CameraPos", camera.position);
	shader->setVec2("CameraRange", camera.getRange());
	shader->setVec2("WindowSize", camera.window->getWindowSize());
	shader->setVec3("SpherePos", attributes->position);
	shader->setVec3("Scale", attributes->scale);

	for(auto& mesh : *meshes)
		mesh->Draw(shader);
}

void Primitives::RMSphere::update(Camera::Camera& camera) {

}

Primitives::Cone::Cone(ObjectAttributes::Common* attributes, const std::string& vertFilePath, const std::string& fragFilePath) : Object(attributes)
{
	auto shaderPair = ResourceManager::createOrGetShader({vertFilePath, fragFilePath});
	this->shader = shaderPair.second;
	this->setShaderResourceKey(shaderPair.first);

	auto meshPair = ResourceManager::createOrGetMesh("../Shadem/Assets/Default_Resources/Models/cone.obj");
	this->meshes = meshPair.second;
	this->setMeshResourceKey(meshPair.first);
}

void Primitives::Cone::update(Camera::Camera& camera)
{
}

void Primitives::Cone::setupMesh()
{
}

Primitives::Torus::Torus(ObjectAttributes::Common* attributes, const std::string& vertFilePath, const std::string& fragFilePath) : Object(attributes)
{
	auto shaderPair = ResourceManager::createOrGetShader({vertFilePath, fragFilePath});
	this->shader = shaderPair.second;
	this->setShaderResourceKey(shaderPair.first);

	auto meshPair = ResourceManager::createOrGetMesh("../Shadem/Assets/Default_Resources/Models/torus.obj");
	this->meshes = meshPair.second;
	this->setMeshResourceKey(meshPair.first);
}

void Primitives::Torus::update(Camera::Camera& camera)
{
}

void Primitives::Torus::setupMesh()
{
}

Primitives::Cylinder::Cylinder(ObjectAttributes::Common* attributes, const std::string& vertFilePath, const std::string& fragFilePath) : Object(attributes)
{
	auto shaderPair = ResourceManager::createOrGetShader({vertFilePath, fragFilePath});
	this->shader = shaderPair.second;
	this->setShaderResourceKey(shaderPair.first);

	auto meshPair = ResourceManager::createOrGetMesh("../Shadem/Assets/Default_Resources/Models/cylinder.obj");
	this->meshes = meshPair.second;
	this->setMeshResourceKey(meshPair.first);
}

void Primitives::Cylinder::update(Camera::Camera& camera)
{
}

void Primitives::Cylinder::setupMesh()
{
}

Primitives::Sphere::Sphere(ObjectAttributes::Common* attributes, const std::string& vertFilePath, const std::string& fragFilePath) : Object(attributes)
{
	auto shaderPair = ResourceManager::createOrGetShader({vertFilePath, fragFilePath});
	this->shader = shaderPair.second;
	this->setShaderResourceKey(shaderPair.first);

	auto meshPair = ResourceManager::createOrGetMesh("../Shadem/Assets/Default_Resources/Models/sphere.obj");
	this->meshes = meshPair.second;
	this->setMeshResourceKey(meshPair.first);
}

void Primitives::Sphere::update(Camera::Camera& camera)
{
}

void Primitives::Sphere::setupMesh()
{
}

// TODO: Circle object should gen vertex buffer for lines and draw gl_lines
Primitives::Circle::Circle(ObjectAttributes::Common* attributes, const std::string& vertFilePath, const std::string& fragFilePath) : Object(attributes)
{
	auto shaderPair = ResourceManager::createOrGetShader({vertFilePath, fragFilePath});
	this->shader = shaderPair.second;
	this->setShaderResourceKey(shaderPair.first);

	auto meshPair = ResourceManager::createOrGetMesh("../Shadem/Assets/Default_Resources/Models/circle.obj");
	this->meshes = meshPair.second;
	this->setMeshResourceKey(meshPair.first);
}

void Primitives::Circle::update(Camera::Camera& camera)
{
}

void Primitives::Circle::setupMesh()
{
}
