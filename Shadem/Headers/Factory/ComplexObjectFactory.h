#pragma once

#include "Objects/Object.h"
#include "Objects/ObjectTypes.h"

class ComplexObjectFactory {
public:
	Object* createObject(ObjectTypes::ComplexObjectType::Type objectType, ObjectBasicAttributes objectBasicAttribute);
};