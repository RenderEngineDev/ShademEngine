#pragma once

#include "Objects/Mesh.h"
#include "Objects/Attributes.h"
#include "Shader/Shader.h"
#include "Camera/Camera.h"
#include "ResourceManager/ResourceManager.h"

enum RenderType
{
	StandardRender,
	RayMarchingRender,
	CubeMarchingRender
};


class Object {

protected:
	ObjectAttributes::Common* attributes;

	// float yaw;
	// float pitch;

	// glm::vec3 front;
	// glm::vec3 up;
	// glm::vec3 right;
	// glm::vec3 worldUp;

	std::shared_ptr<Shader> shader = nullptr;
	std::shared_ptr<std::vector<Mesh*>> meshes = nullptr;

	RenderType renderer = RenderType::StandardRender;

private:

	std::string meshResourceKey;
	std::string shaderResourceKey;

	virtual void setupMesh() = 0;

public:

	Object() {};
	Object(ObjectAttributes::Common* attributes);

	Object(const std::string& filePath ,ObjectAttributes::Common* attributes = new ObjectAttributes::Common());

	virtual void draw(Camera::Camera &camera);
	virtual void update(Camera::Camera& camera) = 0;

	virtual ObjectAttributes::Common* getAttributes() const { 
		return attributes; 
	}

	~Object() {
		shader.reset();
		meshes.reset();

		ResourceManager::tryDeleteMesh(meshResourceKey);
		ResourceManager::tryDeleteShader(shaderResourceKey);
		
		delete attributes;
	}

	RenderType& getRenderType() { return renderer; }

	void setMeshResourceKey(const std::string& key) { meshResourceKey = key; }
	void setShaderResourceKey(const std::string& key) { shaderResourceKey = key; }

};