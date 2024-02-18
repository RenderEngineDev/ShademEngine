#pragma once
#include "Factory/LightObjectFactory.h"

using namespace ObjectTypes;

Object* LightObjectFactory::createObject(LightObjectType::Type objectType, ObjectAttributes::Common* attributes) {
	// TODO: trzeba zaimplementowa� obiekty �wietlne :)
	switch (objectType) {
		case ObjectTypes::LightObjectType::POINT_LIGHT:
			return nullptr;
		case ObjectTypes::LightObjectType::DIRECTION_LIGHT:
			return nullptr;
		case ObjectTypes::LightObjectType::POSITION_LIGHT:
			return nullptr;
		case ObjectTypes::LightObjectType::SPOT_LIGHT:
			return nullptr;
		default:
			return nullptr;
	}
}
