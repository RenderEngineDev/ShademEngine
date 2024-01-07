#pragma once

#include "Scene/Level/CustomLevel.h"


CustomLevel::CustomLevel() {
	configure();
};

int CustomLevel::configure() {
	camera = new Camera::Camera(glm::vec3(0.0f, 1.0f, -10.0f));
	return 1;
}

void CustomLevel::initBasicObjects() {
	createObject(ObjectTypes::PrimitiveObjectType::CUBE, ObjectBasicAttributes(glm::vec3(0.0f, 0.0f, 35.0f), glm::vec3(2.0f)));
	createObject(ObjectTypes::PrimitiveObjectType::PLANE, ObjectBasicAttributes(glm::vec3(0.0f, -15.0f, 35.0f), glm::vec3(5.0f)));
}

CustomLevel::~CustomLevel() {
	delete camera;
}
