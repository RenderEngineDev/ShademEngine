#pragma once

#include "Objects/Object.h"
#include "Objects/ObjectTypes.h"

class Factory {
	virtual Object* createObject(ObjectTypes::Enum objectType, ObjectBasicAttributes objectBasicAttribute) = 0;
};