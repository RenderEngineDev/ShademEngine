#pragma once

#include "Scene/Scene.h"

Scene::Scene() {
	configure();
}

Scene::~Scene() {
	delete camera;
}

int Scene::configure() {
	if (camera)
		delete camera;
	camera = new Camera::Camera(glm::vec3(0.0f, 5.0f, -25.0f));
	return 1;
}

void Scene::initBasicObjects() {}

int Scene::getNumberOfObjects() { 
	return objects.size();
}

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

