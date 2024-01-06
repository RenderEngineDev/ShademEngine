#pragma once

#include "Objects/Object.h"
#include "Objects/ObjectTypes.h"
#include "Objects/Attributes.h"

class RayMarchingObjectFactory {
public:
	static Object* createObject(ObjectTypes::RayMarchingObjectType::Type objectType, ObjectAttributes::Common* attributes);
};