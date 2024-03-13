#pragma once

#include <vector>
#include "Camera/Camera.h"
#include "Objects/Object.h"
#include "Objects/ObjectTypes.h"
#include "Objects/Primitives.h"
#include "Factory/Factory.h"
#include "Factory/PrimitiveObjectFactory.h"
#include "Factory/ComplexObjectFactory.h"
#include "Factory/LightObjectFactory.h"
#include "Factory/CubeMarchingObjectFactory.h"
#include "Factory/RayMarchingObjectFactory.h"
#include "Objects/Attributes.h"

// TODO: do przeniesienia (z ShademEngine.cpp te¿) - do window?

class Scene {
protected:
	// TODO: pointery wszystkie do zamiany bêd¹ na smart pointery
	std::vector<Object*> objects{};
	Camera::Camera* camera = nullptr;

public:
	Scene();

	virtual int configure() = 0;
	virtual void initBasicObjects() = 0;

	void update();
	//int draw();
	bool addObject(Object* object);
	int getNumberOfObjects();

	template<class FactoryType, class ObjectEnumType, class AttributeType>
	inline bool createObject(ObjectEnumType objectType, AttributeType* attributes) {
		return addObject(FactoryType::createObject(objectType.getType(), attributes));
	}
	
	Camera::Camera* getCamera();
	std::vector<Object*>& getObjects();

	~Scene();

};