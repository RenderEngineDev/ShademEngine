#pragma once

#include "Scene/Level/CmLevel.h"


CmLevel::CmLevel() {
	configure();
};

int CmLevel::configure() {
	camera = new Camera::Camera(glm::vec3(0.0f, 15.0f, -35.0f));
	return 1;
}

void CmLevel::initBasicObjects() {
	createObject(ObjectTypes::PrimitiveObjectType::CUBE, ObjectBasicAttributes(glm::vec3(0.0f, 0.0f, 35.0f), glm::vec3(2.0f)));
	createObject(ObjectTypes::PrimitiveObjectType::PLANE, ObjectBasicAttributes(glm::vec3(0.0f, -15.0f, 35.0f), glm::vec3(5.0f)));
	createObject(ObjectTypes::CubeMarchingObjectType::SPHERE, CubeMarchingAttributes(glm::vec3(-25.0f, -15.0f, 35.0f), glm::vec3(1.0f), glm::vec3(0.0f), glm::vec3(50.0f), 1.0f, 5.0f));
	createObject(ObjectTypes::CubeMarchingObjectType::RANDOM, CubeMarchingAttributes(glm::vec3(25.0f, -15.0f, 35.0f), glm::vec3(1.0f), glm::vec3(0.0f), glm::vec3(40.0f), 0.70f));
}

CmLevel::~CmLevel() {
	delete camera;
}
