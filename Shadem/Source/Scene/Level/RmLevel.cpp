#pragma once

#include "Scene/Level/RmLevel.h"


RmLevel::RmLevel() {
	configure();
};

int RmLevel::configure() {
	camera = new Camera::Camera(glm::vec3(0.0f, 1.0f, -10.0f));
	return 1;
}

void RmLevel::initBasicObjects() {
	createObject<PrimitiveObjectFactory, ObjectTypes::PrimitiveObjectType, ObjectAttributes::Common>(
		ObjectTypes::PrimitiveObjectType(ObjectTypes::PrimitiveObjectType::CUBE), 
		new ObjectAttributes::Common(glm::vec3(0.0f, 0.0f, 35.0f), glm::vec3(2.0f)));
	createObject<PrimitiveObjectFactory, ObjectTypes::PrimitiveObjectType, ObjectAttributes::Common>(
		ObjectTypes::PrimitiveObjectType(ObjectTypes::PrimitiveObjectType::CUBE), 
		new ObjectAttributes::Common(glm::vec3(0.0f, 5.0f, 35.0f), glm::vec3(2.0f)));
	createObject<RayMarchingObjectFactory, ObjectTypes::RayMarchingObjectType, ObjectAttributes::Sphereous>(
		ObjectTypes::RayMarchingObjectType(ObjectTypes::RayMarchingObjectType::SPHERE), 
		new ObjectAttributes::Sphereous(glm::vec3(2.0f, 0.0f, 35.0f), glm::vec3(2.0f, 0.0f, 0.0f)));
	createObject<RayMarchingObjectFactory, ObjectTypes::RayMarchingObjectType, ObjectAttributes::Sphereous>(
		ObjectTypes::RayMarchingObjectType(ObjectTypes::RayMarchingObjectType::SPHERE), 
		new ObjectAttributes::Sphereous(glm::vec3(-5.0f, 0.0f, 35.0f), glm::vec3(2.0f, 0.0f, 0.0f)));
	createObject<PrimitiveObjectFactory, ObjectTypes::PrimitiveObjectType, ObjectAttributes::Common>(
		ObjectTypes::PrimitiveObjectType(ObjectTypes::PrimitiveObjectType::CUBE),
		new ObjectAttributes::Common(glm::vec3(0.0f, -5.0f, 35.0f), glm::vec3(2.0f)));
}

RmLevel::~RmLevel() {
	delete camera;
}
