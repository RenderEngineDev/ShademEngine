#pragma once
#include "Factory/RayMarchingObjectFactory.h"

using namespace ObjectTypes;

Object* RayMarchingObjectFactory::createObject(RayMarchingObjectType::Type objectType, ObjectBasicAttributes objectBasicAttribute) {
	// TODO:
	switch (objectType) {
		case ObjectTypes::RayMarchingObjectType::SPHERE:
			return nullptr;
		case ObjectTypes::RayMarchingObjectType::CUBE:
			return nullptr;
		default:
			return nullptr;
	}
}