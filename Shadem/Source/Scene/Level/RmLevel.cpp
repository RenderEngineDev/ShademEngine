#pragma once

#include "Scene/Level/RmLevel.h"

void RmLevel::initBasicObjects() {
	//createObject<PrimitiveObjectFactory, ObjectTypes::PrimitiveObjectType, ObjectAttributes::Common>(
	//	ObjectTypes::PrimitiveObjectType(ObjectTypes::PrimitiveObjectType::CUBE), 
	//	new ObjectAttributes::Common(glm::vec3(0.0f, 0.0f, 35.0f), glm::vec3(2.0f)));
	createObject<PrimitiveObjectFactory, ObjectTypes::PrimitiveObjectType, ObjectAttributes::Common>(
		ObjectTypes::PrimitiveObjectType(ObjectTypes::PrimitiveObjectType::CUBE), 
		new ObjectAttributes::Common(glm::vec3(0.0f, 5.0f, 35.0f), glm::vec3(2.0f)));
	createObject<RayMarchingObjectFactory, ObjectTypes::RayMarchingObjectType, ObjectAttributes::Sphereous>(
		ObjectTypes::RayMarchingObjectType(ObjectTypes::RayMarchingObjectType::SPHERE), 
		new ObjectAttributes::Sphereous(glm::vec3(2.0f, 0.0f, 35.0f), glm::vec3(2.0f, 0.0f, 0.0f)));
	createObject<RayMarchingObjectFactory, ObjectTypes::RayMarchingObjectType, ObjectAttributes::Sphereous>(
		ObjectTypes::RayMarchingObjectType(ObjectTypes::RayMarchingObjectType::SPHERE), 
		new ObjectAttributes::Sphereous(glm::vec3(-5.0f, 0.0f, 35.0f), glm::vec3(2.0f, 0.0f, 0.0f)));
	//createObject<PrimitiveObjectFactory, ObjectTypes::PrimitiveObjectType, ObjectAttributes::Common>(
	//	ObjectTypes::PrimitiveObjectType(ObjectTypes::PrimitiveObjectType::CUBE),
	//	new ObjectAttributes::Common(glm::vec3(0.0f, -5.0f, 35.0f), glm::vec3(2.0f)));

	//for (int i = 0; i < 10; ++i) {
	//	for (int j = 0; j < 10; ++j) {
	//		/*createObject<RayMarchingObjectFactory, ObjectTypes::RayMarchingObjectType, ObjectAttributes::Sphereous>(
	//			ObjectTypes::RayMarchingObjectType(ObjectTypes::RayMarchingObjectType::SPHERE),
	//			new ObjectAttributes::Sphereous(glm::vec3(-5.0f - 5.0f * i, 5.0f * j, 35.0f), glm::vec3(2.0f, 0.0f, 0.0f)));*/

	//		createObject<PrimitiveObjectFactory, ObjectTypes::PrimitiveObjectType, ObjectAttributes::Common>(
	//			ObjectTypes::PrimitiveObjectType(ObjectTypes::PrimitiveObjectType::CUBE),
	//			new ObjectAttributes::Common(glm::vec3(-5.0f * i, 5.0f * j, 35.0f), glm::vec3(2.0f)));

	//	}
	//}
}
