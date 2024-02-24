#pragma once

#include "Scene/Level/CustomLevel.h"


//CustomLevel::CustomLevel() {
//	configure();
//};
//
//int CustomLevel::configure() {
//	if (camera)
//		delete camera;
//	camera = new Camera::Camera(glm::vec3(0.0f, 1.0f, -10.0f));
//	return 1;
//}

void CustomLevel::initBasicObjects() {
	createObject<PrimitiveObjectFactory, ObjectTypes::PrimitiveObjectType, ObjectAttributes::Common>(ObjectTypes::PrimitiveObjectType(ObjectTypes::PrimitiveObjectType::CUBE), new ObjectAttributes::Common(glm::vec3(0.0f, 0.0f, 35.0f), glm::vec3(2.0f)));
	createObject<PrimitiveObjectFactory, ObjectTypes::PrimitiveObjectType, ObjectAttributes::Common>(ObjectTypes::PrimitiveObjectType(ObjectTypes::PrimitiveObjectType::PLANE), new ObjectAttributes::Common(glm::vec3(0.0f, -15.0f, 35.0f), glm::vec3(10.0f)));
	createObject<PrimitiveObjectFactory, ObjectTypes::PrimitiveObjectType, ObjectAttributes::Common>(ObjectTypes::PrimitiveObjectType(ObjectTypes::PrimitiveObjectType::CUBE), new ObjectAttributes::Common(glm::vec3(5.0f, 0.0f, 35.0f), glm::vec3(0.2f)));

	createObject<PrimitiveObjectFactory, ObjectTypes::PrimitiveObjectType, ObjectAttributes::Common>(ObjectTypes::PrimitiveObjectType(ObjectTypes::PrimitiveObjectType::CONE), new ObjectAttributes::Common(glm::vec3(-5.0f, 0.0f, 35.0f), glm::vec3(2.0f)));
	createObject<PrimitiveObjectFactory, ObjectTypes::PrimitiveObjectType, ObjectAttributes::Common>(ObjectTypes::PrimitiveObjectType(ObjectTypes::PrimitiveObjectType::TORUS), new ObjectAttributes::Common(glm::vec3(-10.0f, 0.0f, 35.0f), glm::vec3(2.0f)));
	createObject<PrimitiveObjectFactory, ObjectTypes::PrimitiveObjectType, ObjectAttributes::Common>(ObjectTypes::PrimitiveObjectType(ObjectTypes::PrimitiveObjectType::CYLINDER), new ObjectAttributes::Common(glm::vec3(10.0f, 0.0f, 35.0f), glm::vec3(2.0f)));
	createObject<PrimitiveObjectFactory, ObjectTypes::PrimitiveObjectType, ObjectAttributes::Common>(ObjectTypes::PrimitiveObjectType(ObjectTypes::PrimitiveObjectType::SPHERE), new ObjectAttributes::Common(glm::vec3(-15.0f, 0.0f, 35.0f), glm::vec3(2.0f)));
	createObject<PrimitiveObjectFactory, ObjectTypes::PrimitiveObjectType, ObjectAttributes::Common>(ObjectTypes::PrimitiveObjectType(ObjectTypes::PrimitiveObjectType::CIRCLE), new ObjectAttributes::Common(glm::vec3(-20.0f, 0.0f, 35.0f), glm::vec3(2.0f), glm::vec3(90.0f,0.0f,0.0f)));
	
	//createObject<PrimitiveObjectFactory, ObjectTypes::PrimitiveObjectType, ObjectAttributes::Common>(ObjectTypes::PrimitiveObjectType(ObjectTypes::PrimitiveObjectType::PLANE), new ObjectAttributes::Common(glm::vec3(0.0f, -15.0f, 35.0f), glm::vec3(5.0f)));
}

