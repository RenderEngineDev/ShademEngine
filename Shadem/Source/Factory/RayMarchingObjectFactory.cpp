#pragma once
#include "Factory/RayMarchingObjectFactory.h"
#include "Objects/Primitives.h"

using namespace ObjectTypes;

Object* RayMarchingObjectFactory::createObject(RayMarchingObjectType::Type objectType, ObjectBasicAttributes objectBasicAttribute) {
	// TODO:
	switch (objectType) {
		case ObjectTypes::RayMarchingObjectType::SPHERE:
			return new Primitives::RMSphere(objectBasicAttribute.position, objectBasicAttribute.scale, Shaders::getRmSphereFragmentShaderPath());
		case ObjectTypes::RayMarchingObjectType::CUBE:
			return nullptr;
		default:
			return nullptr;
	}
}