#pragma once

#include "Objects/Object.h"
#include "Objects/ObjectTypes.h"
#include "Objects/Attributes.h"

class ComplexObjectFactory {
public:
	static Object* createObject(ObjectTypes::ComplexObjectType::Type objectType, ObjectAttributes::Common* attributes);
};