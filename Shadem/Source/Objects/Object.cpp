#pragma once

#include "Objects/Object.h"

Object::Object(ObjectAttributes::Common* attributes) : attributes(attributes) {

}

void Object::draw(Camera::Camera& camera)
{
	shader->use();

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, attributes->position);
	model = glm::scale(model, attributes->scale);

	model = glm::rotate(model, glm::radians(attributes->rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(attributes->rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(attributes->rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

	shader->setMat4("projection", camera.getProjection());
	shader->setMat4("view", camera.getView());
	shader->setMat4("model", model);

	for (auto& mesh : *meshes)
		mesh->Draw(shader);
}
