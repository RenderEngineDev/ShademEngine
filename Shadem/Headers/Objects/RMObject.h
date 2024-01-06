#pragma once

#include "Object.h"
#include "Camera/Camera.h"

class RMObject : public Object {

protected:

	//std::string sDF;
	//std::string mainContent;

	/*
		// Do przysz³ej edycji obiektów
		glm::vec3 Front;
		glm::vec3 Up;
		glm::vec3 Right;
	*/


	Shader shader;

private:
	

	RenderType renderer = RenderType::RayMarchingRender;
	virtual void setupMesh() override;

public:
	RMObject(ObjectAttributes::Common* attributes, const char* fShaderPath);
	virtual void draw(Camera::Camera& camera) = 0;
	~RMObject() {
	}

	//bool LoadSDF(const std::string& path);
	//bool LoadMainContent(const std::string& path);


};