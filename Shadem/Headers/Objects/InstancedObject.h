#pragma once

#include "Object.h"
#include "ShademApplication/ShademEngine.h"

class InstancedObject : public Object
{
private:
	unsigned int VBO;
	std::vector<glm::mat4> mvps;

	void init();

protected:
	std::vector<ObjectAttributes::Common*> instancesAttributes;
public:
	InstancedObject();
	InstancedObject(const std::string& modelPath);
	InstancedObject(const std::vector<ObjectAttributes::Common*>& _instancesAttributes);
	InstancedObject(const std::vector<ObjectAttributes::Common*>& _instancesAttributes, const std::string& modelPath);

	~InstancedObject();

	void setInstacesAttributes(const std::vector<ObjectAttributes::Common*>& _instancesAttributes);
	void setInstacesAttributes(int index, const std::vector<ObjectAttributes::Common*>& _instancesAttributes);
	void setInstacesAttributes(int index, ObjectAttributes::Common* _instancesAttributes);
	void setMesh(const std::shared_ptr<std::vector<Mesh*>>& model, const std::string& name);
	void setMesh(const std::string& modelPath);

	virtual void draw(Camera::Camera& camera) override;
	void setupMesh();
	void update(Camera::Camera& camera);
};