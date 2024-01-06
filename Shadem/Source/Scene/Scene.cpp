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

//int Scene::draw() {
//	for (Object* object : objects) {
//		object->draw(*camera);
//	}
//	return 1;
//}

void Scene::update() {
	for (Object* object : objects) {
		object->update(*camera);
	}
}

bool Scene::addObject(Object* object) {
	if (object) {
		objects.push_back(std::move(object));
		return true;
	}
	return false;
}

Camera::Camera* Scene::getCamera() {
	return camera;
}

std::vector<Object*>& Scene::getObjects() {
	return objects;
}

Scene::~Scene() {

	for (auto object : objects) {
		delete object;
	}
	delete camera;
}
