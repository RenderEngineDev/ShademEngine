#pragma once

#include "Objects/Object.h"
#include "Objects/ObjectTypes.h"

class CubeMarchingObjectFactory {
public:
	Object* createObject(ObjectTypes::CubeMarchingObjectType::Type objectType, ObjectBasicAttributes objectBasicAttribute);
};