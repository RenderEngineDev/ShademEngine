#include "Objects/Lights/Light.h"
#include "Objects/ObjectTypes.h"

std::vector<Lights::Light*> Lights::Light::lights{};

void Lights::Light::setShaderLights(Shader* shader) {
	// TODO: should be added to objects attributes for fong calculations?
	shader->setInt("material.diffuse", 0);
	shader->setInt("material.specular", 1);
	shader->setInt("material.emission", 2);
	shader->setFloat("material.shininess", 64.0f);

	std::vector<Lights::DirectionalLight*> dirLights = Lights::Light::getLightsByType<Lights::DirectionalLight>((ObjectTypes::LightObjectType::DIRECTION_LIGHT));
	if (dirLights.size() > 0) {
		ObjectAttributes::Lights::DirectionalLight& lightAttributes = *dirLights[0]->getAttributes();
		shader->setVec3("dirLight.direction", lightAttributes.direction);
		shader->setVec3("dirLight.ambient", lightAttributes.ambient);
		shader->setVec3("dirLight.diffuse", lightAttributes.diffuse);
		shader->setVec3("dirLight.specular", lightAttributes.specular);
	}
	std::vector<Lights::PointLight*> pointLights = Lights::Light::getLightsByType<Lights::PointLight>((ObjectTypes::LightObjectType::POINT_LIGHT));
	if (pointLights.size() > 0) {
		for (int i = 0; i < pointLights.size(); i++) {
			std::string pointType = "pointLights[" + std::to_string(i) + "]";
			ObjectAttributes::Lights::PointLight& lightAttributes = *pointLights[i]->getAttributes();
			shader->setVec3(pointType + ".position", lightAttributes.position);
			shader->setVec3(pointType + ".ambient", lightAttributes.ambient);
			shader->setVec3(pointType + ".diffuse", lightAttributes.diffuse);
			shader->setVec3(pointType + ".specular", lightAttributes.specular);
			shader->setFloat(pointType + ".constant", lightAttributes.constant);
			shader->setFloat(pointType + ".linear", lightAttributes.linear);
			shader->setFloat(pointType + ".quadratic", lightAttributes.quadratic);
		}
	}
}

Lights::Light::Light(ObjectAttributes::Lights::Light* attributes, ObjectTypes::LightObjectType::Type type, std::initializer_list<const std::string> filePaths) : attributes(attributes), type(type), Object(attributes) {
	lights.push_back(this);
	
	auto shaderPair = ResourceManager::createOrGetShader(filePaths);
	this->shader = shaderPair.second;
	this->setShaderResourceKey(shaderPair.first);

	auto meshPair = ResourceManager::createOrGetMesh("../Shadem/Assets/Default_Resources/Models/cube.obj");
	this->meshes = meshPair.second;
	this->setMeshResourceKey(meshPair.first);
}


void Lights::Light::draw(Camera::Camera& camera) {
	shader->use();

	glm::mat4& model = glm::mat4(1.0f);
	evaluateBasicModelTransformations(model);
	shader->setMat4("MVP", camera.getProjection() * camera.getView() * model);

	for (auto& mesh : *meshes)
		mesh->Draw(shader);

}

void Lights::Light::update(Camera::Camera& camera) {
}

void Lights::Light::setupMesh() {
}
