#pragma once

#include "Object.h"

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
	

	virtual void setupMesh() override;

public:
	RMObject(glm::vec3 position, glm::vec3 scale, const char* fShaderPath);
	~RMObject() {
	}

	//bool LoadSDF(const std::string& path);
	//bool LoadMainContent(const std::string& path);


};