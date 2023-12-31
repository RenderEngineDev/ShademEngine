#pragma once

#include "Object.h"
#include "Camera/Camera.h"

class RMObject : public Object {

protected:

	//std::string sDF;
	//std::string mainContent;

	/*
		// Do przysz�ej edycji obiekt�w
		glm::vec3 Front;
		glm::vec3 Up;
		glm::vec3 Right;
	*/


	Shader shader;

private:
	

	virtual void setupMesh() override;

public:
	RMObject(glm::vec3 position, glm::vec3 scale, const char* fShaderPath);
	virtual void draw(Camera::Camera& camera) = 0;
	~RMObject() {
	}

	//bool LoadSDF(const std::string& path);
	//bool LoadMainContent(const std::string& path);


};