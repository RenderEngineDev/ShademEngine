#pragma once

#include "Scene/Scene.h"

Scene::Scene() {
};

int Scene::configure() {
	camera = new Camera::Camera(glm::vec3(0.0f, 5.0f, -25.0f));
	return 1;
}

void Scene::initBasicObjects() {}

int Scene::getNumberOfObjects() { 
	return objects.size();
}

int Scene::draw() {
	for (Object* object : objects) {
		object->draw(*camera);
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

	for (auto object : objects) {
		delete object;
	}
	delete primitiveFactory;
	delete cubeMarchingFactory;
	delete rayMarchingFactory;
	delete complexFactory;
	delete lightFactory;
	delete camera;
}
