#pragma once

#include "Object.h"
#include "Camera/Camera.h"

class RMObject : public Object {

protected:

	void setUniforms(const Camera::Camera& camera);

	//std::string sDF;
	//std::string mainContent;

	/*
		// Do przysz³ej edycji obiektów
		glm::vec3 Front;
		glm::vec3 Up;
		glm::vec3 Right;
	*/


private:
	

	RenderType renderer = RenderType::RayMarchingRender;
	virtual void setupMesh() override;

public:
	RMObject();
	RMObject(ObjectAttributes::Common* attributes);
	virtual void draw(Camera::Camera& camera) = 0;

	//bool LoadSDF(const std::string& path);
	//bool LoadMainContent(const std::string& path);


};