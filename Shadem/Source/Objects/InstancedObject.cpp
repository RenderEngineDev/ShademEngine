#pragma once

#include "Objects/InstancedObject.h"

void InstancedObject::init()
{
	glGenBuffers(1, &VBO);
	auto shaderPair = ResourceManager::createOrGetShader({ Shaders::getCommonInstanceVertexShaderPath(),  Shaders::getCommonInstanceFragmentShaderPath() });
	this->shader = shaderPair.second;
	this->setShaderResourceKey(shaderPair.first);
}

InstancedObject::InstancedObject() : VBO(0)
{
	init();
}

InstancedObject::InstancedObject(const std::vector<ObjectAttributes::Common*>& _instancesAttributes)
{
	init();

	setInstacesAttributes(_instancesAttributes);
	setMesh("../Shadem/Assets/Default_Resources/Models/cube.obj"); 
}

InstancedObject::InstancedObject(const std::string& modelPath)
{
	init();
	setMesh(modelPath);
	setInstacesAttributes(std::vector<ObjectAttributes::Common*>{new ObjectAttributes::Common()});
}

InstancedObject::InstancedObject(const std::vector<ObjectAttributes::Common*>& _instancesAttributes, const std::string& modelPath)
{
	init();
	setInstacesAttributes(_instancesAttributes);
	setMesh(modelPath);
}

InstancedObject::~InstancedObject()
{
	glDeleteBuffers(1, &VBO);
}

void InstancedObject::setInstacesAttributes(const std::vector<ObjectAttributes::Common*>& _instancesAttributes)
{
	instancesAttributes = _instancesAttributes;
	mvps = std::vector<glm::mat4>(instancesAttributes.size(), glm::mat4(1.0f));

	for (int i = 0; i < instancesAttributes.size(); ++i) {
		evaluateBasicModelTransformations(mvps[i], instancesAttributes[i]);
	}

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * mvps.size(), &mvps[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void InstancedObject::setInstacesAttributes(int index, const std::vector<ObjectAttributes::Common*>& _instancesAttributes)
{
	for (int i = 0; i < _instancesAttributes.size(); ++i) {
		instancesAttributes[index + i] = _instancesAttributes[i];
		mvps[index + i] = glm::mat4(1.0f);
		evaluateBasicModelTransformations(mvps[index + i], instancesAttributes[index + i]);
	}
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(GL_ARRAY_BUFFER, index * sizeof(glm::mat4), sizeof(glm::mat4) * _instancesAttributes.size(), &mvps[index]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void InstancedObject::setInstacesAttributes(int index, ObjectAttributes::Common* _instancesAttributes)
{
	instancesAttributes[index] = _instancesAttributes;
	evaluateBasicModelTransformations(mvps[index], instancesAttributes[index]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(GL_ARRAY_BUFFER, index * sizeof(glm::mat4), sizeof(glm::mat4), &mvps[index]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void InstancedObject::setMesh(const std::shared_ptr<std::vector<Mesh*>>& model, const std::string& name)
{
	if (this->meshes)
	{
		meshes.reset();
		ResourceManager::tryDeleteMesh(getMeshResourceKey());
	}

	this->meshes = model;
	this->setMeshResourceKey(name);

	setupMesh();
}

void InstancedObject::setMesh(const std::string& modelPath)
{
	auto meshPair = ResourceManager::createOrGetMesh(modelPath);

	if (this->meshes) 
	{
		meshes.reset();
		ResourceManager::tryDeleteMesh(getMeshResourceKey());
	}

	this->meshes = meshPair.second;
	this->setMeshResourceKey(meshPair.first);

	setupMesh();
}

void InstancedObject::draw(Camera::Camera& camera)
{
	shader->use();

	glm::mat4& model = glm::mat4(1.0f);
	evaluateBasicModelTransformations(model);
	shader->setMat4("MVP", camera.getProjection() * camera.getView() * model);

	for (auto& mesh : *meshes)
		mesh->DrawInstanced(shader, mvps.size());
}

void InstancedObject::setupMesh()
{

	for (auto& mesh : *meshes) {

		glBindVertexArray(mesh->getVAO());

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		
		std::size_t vec4Size = sizeof(glm::vec4);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(1 * vec4Size));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);

		glBindVertexArray(0);
	}

}

void InstancedObject::update(Camera::Camera& camera)
{
}
