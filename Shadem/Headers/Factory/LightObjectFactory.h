#pragma once

#include "Objects/Object.h"
#include "Objects/ObjectTypes.h"
#include "Objects/Attributes.h"

class LightObjectFactory {
public:
	static Object* createObject(ObjectTypes::LightObjectType::Type objectType, ObjectAttributes::Common* attributes);
};