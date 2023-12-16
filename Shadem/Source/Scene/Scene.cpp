#pragma once

#include "Scene/Scene.h"


Scene::Scene() {
	configure();
};

int Scene::configure() {
	camera = new Camera::Camera(glm::vec3(0.0f, 1.0f, -10.0f));
	return 1;
}

void Scene::initBasicObjects() {
	createObject(ObjectTypes::PrimitiveObjectType::CUBE, ObjectBasicAttributes(glm::vec3(0.0f, 0.0f, 35.0f), glm::vec3(2.0f)));
	createObject(ObjectTypes::PrimitiveObjectType::PLANE, ObjectBasicAttributes(glm::vec3(0.0f, -15.0f, 35.0f), glm::vec3(5.0f)));
	//McObject mcObject(MarchingCubeGenerator::StructureType::RANDOM, 50, 0.6f, glm::vec3(15.0f, 0.0f, 45.0f), 65);
	//createObject(ObjectTypes::CubeMarchingObjectType::SPHERE, ObjectBasicAttributes(glm::vec3(0.0f, -15.0f, 35.0f), glm::vec3(5.0f), glm::vec3(0.0f), 10.0f));
}

int Scene::draw(Shader& shader) {

	// FIXME: shader, projection, view <- TYMCZASOWO do przeniesienia do renderera !!!
	shader.use();
	glm::mat4 projection = glm::perspective(glm::radians(camera->zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 250.0f);
	glm::mat4 view = camera->GetViewMatrix();

	for (Object* object : objects) {
		object->Draw(shader, projection, view);
	}
	return 1;
}

bool Scene::addObject(Object* object) {
	if (object) {
		objects.push_back(std::move(object));
		return true;
	}
	return false;
}

bool Scene::createObject(ObjectTypes::PrimitiveObjectType::Type objectType, ObjectBasicAttributes objectBasicAttribute) {
	Object* object = primitiveFactory->createObject(objectType, objectBasicAttribute);
	return addObject(object);
}

bool Scene::createObject(ObjectTypes::LightObjectType::Type objectType, ObjectBasicAttributes objectBasicAttribute) {
	Object* object = lightFactory->createObject(objectType, objectBasicAttribute);
	return addObject(object);
}

bool Scene::createObject(ObjectTypes::ComplexObjectType::Type objectType, ObjectBasicAttributes objectBasicAttribute) {
	Object* object = complexFactory->createObject(objectType, objectBasicAttribute);
	return addObject(object);
}

bool Scene::createObject(ObjectTypes::CubeMarchingObjectType::Type objectType, ObjectBasicAttributes objectBasicAttribute) {
	Object* object = cubeMarchingFactory->createObject(objectType, objectBasicAttribute);
	return addObject(object);
}

bool Scene::createObject(ObjectTypes::RayMarchingObjectType::Type objectType, ObjectBasicAttributes objectBasicAttribute) {
	Object* object = rayMarchingFactory->createObject(objectType, objectBasicAttribute);
	return addObject(object);
}

Camera::Camera* Scene::getCamera() {
	return camera;
}

Scene::~Scene() {
	delete camera;
}
