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
	createObject<PrimitiveObjectFactory, ObjectTypes::PrimitiveObjectType, ObjectAttributes::Common>(
		ObjectTypes::PrimitiveObjectType(ObjectTypes::PrimitiveObjectType::CUBE), 
		new ObjectAttributes::Common(glm::vec3(0.0f, 0.0f, 35.0f), 
			glm::vec3(2.0f), 
			glm::vec3(0.0f), 
			"Cube"));
	//createObject(ObjectTypes::PrimitiveObjectType::PLANE, new ObjectBasicAttributes(glm::vec3(0.0f, -15.0f, 35.0f), glm::vec3(5.0f), glm::vec3(0.0f), "Plane"));
	createObject<CubeMarchingObjectFactory, ObjectTypes::CubeMarchingObjectType, ObjectAttributes::CubeMarching>(
		ObjectTypes::CubeMarchingObjectType(ObjectTypes::CubeMarchingObjectType::SPHERE), 
		new ObjectAttributes::CubeMarching(glm::vec3(-25.0f, -15.0f, 35.0f), 
			glm::vec3(1.0f), 
			glm::vec3(0.0f),
			glm::vec3(50.0f), 
			1.0f, 
			5.0f, "Sphere (CM)"));
	//createObject(ObjectTypes::CubeMarchingObjectType::RANDOM, new CubeMarchingAttributes(glm::vec3(25.0f, -15.0f, 35.0f), glm::vec3(5.0f), glm::vec3(0.0f), glm::vec3(40.0f), 0.70f, "Random (CM)"));
	createObject<CubeMarchingObjectFactory, ObjectTypes::CubeMarchingObjectType, ObjectAttributes::CubeMarching>(
		ObjectTypes::CubeMarchingObjectType(ObjectTypes::CubeMarchingObjectType::NOISE), 
		new ObjectAttributes::CubeMarching(glm::vec3(0.0f, 25.0f, 25.0f), 
			glm::vec3(35.0f), 
			glm::vec3(0.0f), 
			glm::vec3(25.0f), 
			0.3f, 
			1.0f, 
			1.0f, 
			1.0f, 
			0.5f, "Noise (CM)"));
}

CmLevel::~CmLevel() {
	delete camera;
}
