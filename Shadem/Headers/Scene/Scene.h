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

// TODO: do przeniesienia (z ShademEngine.cpp te¿) - do window?
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

class Scene {
protected:
	// TODO: pointery wszystkie do zamiany bêd¹ na smart pointery
	std::vector<Object*> objects{};
	Camera::Camera* camera = nullptr;
	PrimitiveObjectFactory* primitiveFactory = new PrimitiveObjectFactory();
	CubeMarchingObjectFactory* cubeMarchingFactory = new CubeMarchingObjectFactory();
	RayMarchingObjectFactory* rayMarchingFactory = new RayMarchingObjectFactory();
	ComplexObjectFactory* complexFactory = new ComplexObjectFactory();
	LightObjectFactory* lightFactory = new LightObjectFactory();

public:
	Scene();

	virtual int configure() = 0;
	virtual void initBasicObjects() = 0;

	int draw();
	bool addObject(Object* object);
	int getNumberOfObjects();

	bool createObject(ObjectTypes::PrimitiveObjectType::Type objectType, ObjectBasicAttributes objectBasicAttribute);
	bool createObject(ObjectTypes::LightObjectType::Type objectType, ObjectBasicAttributes objectBasicAttribute);
	bool createObject(ObjectTypes::ComplexObjectType::Type objectType, ObjectBasicAttributes objectBasicAttribute);
	bool createObject(ObjectTypes::CubeMarchingObjectType::Type objectType, ObjectBasicAttributes objectBasicAttribute);
	bool createObject(ObjectTypes::RayMarchingObjectType::Type objectType, ObjectBasicAttributes objectBasicAttribute);

	Camera::Camera* getCamera();

	~Scene();

};