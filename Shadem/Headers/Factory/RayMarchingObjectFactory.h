#pragma once

#include "Objects/Object.h"
#include "Objects/ObjectTypes.h"

class RayMarchingObjectFactory {
public:
	Object* createObject(ObjectTypes::RayMarchingObjectType::Type objectType, ObjectBasicAttributes objectBasicAttribute);
};