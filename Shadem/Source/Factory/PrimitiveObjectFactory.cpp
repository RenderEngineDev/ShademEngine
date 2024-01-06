#pragma once
#include "Factory/PrimitiveObjectFactory.h"
#include "Objects/Primitives.h"

using namespace Primitives;
using namespace ObjectTypes;

Object* PrimitiveObjectFactory::createObject(PrimitiveObjectType::Type objectType, ObjectAttributes::Common* attributes) {
	// TODO: implementacja obiektów prymitywnych
	switch (objectType) {
		case PrimitiveObjectType::CUBE:
			return new Cube(attributes, new Shader(Shaders::getCommonVertexShaderPath(), Shaders::getCommonFragmentShaderPath()));
		case PrimitiveObjectType::PLANE:
			return new Plane(attributes, new Shader(Shaders::getCommonVertexShaderPath(), Shaders::getCommonFragmentShaderPath()));
		case PrimitiveObjectType::SPHERE:
			return nullptr;
		case PrimitiveObjectType::CIRCLE:
			return nullptr;
		case PrimitiveObjectType::CONE:
			return nullptr;
		case PrimitiveObjectType::CYLINDER:
			return nullptr;
		case PrimitiveObjectType::TORUS:
			return nullptr;
		default:
			return nullptr;
		}
}
