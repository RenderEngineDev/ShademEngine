#pragma once
#include "Factory/RayMarchingObjectFactory.h"
#include "Objects/Primitives.h"

using namespace ObjectTypes;

Object* RayMarchingObjectFactory::createObject(RayMarchingObjectType::Type objectType, ObjectAttributes::Common* attributes) {
	// TODO:
	switch (objectType) {
		case ObjectTypes::RayMarchingObjectType::SPHERE:
			return new Primitives::RMSphere(dynamic_cast<ObjectAttributes::Sphereous*>(attributes), Shaders::getRmSphereFragmentShaderPath());
		case ObjectTypes::RayMarchingObjectType::CUBE:
			return nullptr;
		default:
			return nullptr;
	}
}