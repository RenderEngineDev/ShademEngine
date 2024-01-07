#pragma once
#include "Factory/PrimitiveObjectFactory.h"
#include "Objects/Primitives.h"

using namespace Primitives;
using namespace ObjectTypes;

Object* PrimitiveObjectFactory::createObject(PrimitiveObjectType::Type objectType, ObjectBasicAttributes objectBasicAttribute) {
	// TODO: implementacja obiektów prymitywnych
	switch (objectType) {
		case PrimitiveObjectType::CUBE:
			return new Cube(new Shader(Shaders::getCommonVertexShaderPath(), Shaders::getCommonFragmentShaderPath()), objectBasicAttribute.position, objectBasicAttribute.scale);
			//return new Cube(objectBasicAttribute.position, objectBasicAttribute.scale);
		case PrimitiveObjectType::PLANE:
			return new Plane(new Shader(Shaders::getCommonVertexShaderPath(), Shaders::getCommonFragmentShaderPath()), objectBasicAttribute.position, objectBasicAttribute.scale);
			//return new Plane(objectBasicAttribute.position, objectBasicAttribute.scale);
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
