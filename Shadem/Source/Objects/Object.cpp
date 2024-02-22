#pragma once

#include "Objects/Object.h"

Object::Object(ObjectAttributes::Common* attributes) : attributes(attributes) {

}

void Object::draw(Camera::Camera& camera) {
	shader->use();

	glm::mat4& model = glm::mat4(1.0f);
	evaluateBasicModelTransformations(model);
	shader->setMat4("MVP", camera.getProjection() * camera.getView() * model);
	shader->setVec3("cameraPosition", camera.position);

	for (auto& mesh : *meshes)
		mesh->Draw(shader);
}

void Object::evaluateBasicModelTransformations(glm::mat4& model) {
	model = glm::translate(model, attributes->position);
	model = glm::rotate(model, glm::radians(attributes->rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(attributes->rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(attributes->rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, attributes->scale);
}
