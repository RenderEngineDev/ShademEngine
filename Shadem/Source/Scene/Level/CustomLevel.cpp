#pragma once

#include "Scene/Level/CustomLevel.h"
#include "Objects/InstancedObject.h"


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
	//createObject<PrimitiveObjectFactory, ObjectTypes::PrimitiveObjectType, ObjectAttributes::Common>(ObjectTypes::PrimitiveObjectType(ObjectTypes::PrimitiveObjectType::CUBE), new ObjectAttributes::Common(glm::vec3(0.0f, 0.0f, 35.0f), glm::vec3(2.0f)));
	//createObject<PrimitiveObjectFactory, ObjectTypes::PrimitiveObjectType, ObjectAttributes::Common>(ObjectTypes::PrimitiveObjectType(ObjectTypes::PrimitiveObjectType::PLANE), new ObjectAttributes::Common(glm::vec3(0.0f, -15.0f, 35.0f), glm::vec3(10.0f)));
	//createObject<PrimitiveObjectFactory, ObjectTypes::PrimitiveObjectType, ObjectAttributes::Common>(ObjectTypes::PrimitiveObjectType(ObjectTypes::PrimitiveObjectType::CUBE), new ObjectAttributes::Common(glm::vec3(5.0f, 0.0f, 35.0f), glm::vec3(0.2f)));

	//createObject<PrimitiveObjectFactory, ObjectTypes::PrimitiveObjectType, ObjectAttributes::Common>(ObjectTypes::PrimitiveObjectType(ObjectTypes::PrimitiveObjectType::CONE), new ObjectAttributes::Common(glm::vec3(-5.0f, 0.0f, 35.0f), glm::vec3(2.0f)));
	//createObject<PrimitiveObjectFactory, ObjectTypes::PrimitiveObjectType, ObjectAttributes::Common>(ObjectTypes::PrimitiveObjectType(ObjectTypes::PrimitiveObjectType::TORUS), new ObjectAttributes::Common(glm::vec3(-10.0f, 0.0f, 35.0f), glm::vec3(2.0f)));
	//createObject<PrimitiveObjectFactory, ObjectTypes::PrimitiveObjectType, ObjectAttributes::Common>(ObjectTypes::PrimitiveObjectType(ObjectTypes::PrimitiveObjectType::CYLINDER), new ObjectAttributes::Common(glm::vec3(10.0f, 0.0f, 35.0f), glm::vec3(2.0f)));
	//createObject<PrimitiveObjectFactory, ObjectTypes::PrimitiveObjectType, ObjectAttributes::Common>(ObjectTypes::PrimitiveObjectType(ObjectTypes::PrimitiveObjectType::SPHERE), new ObjectAttributes::Common(glm::vec3(-15.0f, 0.0f, 35.0f), glm::vec3(2.0f)));
	//createObject<PrimitiveObjectFactory, ObjectTypes::PrimitiveObjectType, ObjectAttributes::Common>(ObjectTypes::PrimitiveObjectType(ObjectTypes::PrimitiveObjectType::CIRCLE), new ObjectAttributes::Common(glm::vec3(-20.0f, 0.0f, 35.0f), glm::vec3(2.0f), glm::vec3(90.0f,0.0f,0.0f)));

	// ------------------------------------------------------------------------------------------------------------------------------

	int gridSize = 100;

	std::vector<ObjectAttributes::Common*> attr;
	attr.reserve(gridSize * gridSize * gridSize);
	for (int x = 0; x < gridSize; ++x) {
		for (int y = 0; y < gridSize; ++y) {
			attr.emplace_back(new ObjectAttributes::Common());
			attr.back()->position = glm::vec3(x * 2.5, y * 2.5, 2.5);
		}
	}
	InstancedObject* obj = new InstancedObject(attr, "../Shadem/Assets/Default_Resources/Models/cube.obj");
	addObject(obj);

	// ------------------------------------------------------------------------------------------------------------------------------

	//for (int x = 0; x < gridSize; ++x) {
	//	for (int y = 0; y < gridSize; ++y) {
	//		createObject<PrimitiveObjectFactory, ObjectTypes::PrimitiveObjectType, ObjectAttributes::Common>(ObjectTypes::PrimitiveObjectType(ObjectTypes::PrimitiveObjectType::CUBE), new ObjectAttributes::Common(glm::vec3(x * 2.5, y * 2.5, 0.0f), glm::vec3(1.0f)));
	//	}
	//}
}

