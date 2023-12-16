#pragma once

#include "Objects/Object.h"
#include "Objects/ObjectTypes.h"

class LightObjectFactory {
public:
	Object* createObject(ObjectTypes::LightObjectType::Type objectType, ObjectBasicAttributes objectBasicAttribute);
};