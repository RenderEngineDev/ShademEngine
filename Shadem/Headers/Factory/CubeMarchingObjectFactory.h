#pragma once

#include "Objects/Object.h"
#include "Objects/ObjectTypes.h"
#include "Objects/Attributes.h"

class CubeMarchingObjectFactory {
public:
	static Object* createObject(ObjectTypes::CubeMarchingObjectType::Type objectType, ObjectAttributes::Common* attributes);
};