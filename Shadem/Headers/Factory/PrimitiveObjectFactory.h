#pragma once

#include "Objects/Object.h"
#include "Objects/ObjectTypes.h"

class PrimitiveObjectFactory {
public:
	Object* createObject(ObjectTypes::PrimitiveObjectType::Type objectType, ObjectBasicAttributes objectBasicAttribute);
};