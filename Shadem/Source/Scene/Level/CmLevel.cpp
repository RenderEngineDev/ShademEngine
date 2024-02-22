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
		new ObjectAttributes::CubeMarching(
			glm::vec3(-25.0f, -15.0f, 35.0f),	// position
			glm::vec3(1.0f),					// scale
			glm::vec3(0.0f),					// rotation
			glm::vec3(50.0f),					// gridSize
			1.0f,								// isoValue
			5.0f,								// radius
			"Sphere (CM)"));
	//createObject(ObjectTypes::CubeMarchingObjectType::RANDOM, new CubeMarchingAttributes(glm::vec3(25.0f, -15.0f, 35.0f), glm::vec3(5.0f), glm::vec3(0.0f), glm::vec3(40.0f), 0.70f, "Random (CM)"));
	createObject<CubeMarchingObjectFactory, ObjectTypes::CubeMarchingObjectType, ObjectAttributes::CubeMarching>(
		ObjectTypes::CubeMarchingObjectType(ObjectTypes::CubeMarchingObjectType::NOISE), 
		new ObjectAttributes::CubeMarching(
			glm::vec3(0.0f, 25.0f, 25.0f),  //	position
			glm::vec3(35.0f),				//	scale
			glm::vec3(0.0f),				//	rotation
			glm::vec3(60),					//	gridSize
			0.05f,							//	isoValue
			1.0f,							//	noiseScale
			3.0f,							//	frequency
			1.0f,							//	amplitude
			1.0f,							//	lacunarity
			0.5f,							//	persistance
			"Noise (CM)")); 
}

CmLevel::~CmLevel() {
	delete camera;
}
