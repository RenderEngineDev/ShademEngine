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
	//McObject mcObject(MarchingCubeGenerator::StructureType::RANDOM, 50, 0.6f, glm::vec3(15.0f, 0.0f, 45.0f), 65);
	createObject(ObjectTypes::CubeMarchingObjectType::SPHERE, ObjectBasicAttributes(glm::vec3(0.0f, -15.0f, 35.0f), glm::vec3(5.0f), glm::vec3(0.0f), 10.0f));
}

CmLevel::~CmLevel() {
	delete camera;
}
