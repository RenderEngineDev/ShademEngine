#pragma once

#include "Objects/Object.h"
#include "Objects/ObjectTypes.h"
#include "Objects/Attributes.h"

class PrimitiveObjectFactory {
public:
	static Object* createObject(ObjectTypes::PrimitiveObjectType::Type objectType, ObjectAttributes::Common* attributes);
};