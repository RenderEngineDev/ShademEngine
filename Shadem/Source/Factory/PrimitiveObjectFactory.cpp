#pragma once
#include "Factory/PrimitiveObjectFactory.h"
#include "Objects/Primitives.h"

using namespace Primitives;
using namespace ObjectTypes;

Object* PrimitiveObjectFactory::createObject(PrimitiveObjectType::Type objectType, ObjectAttributes::Common* attributes) {
	// TODO: implementacja obiektów prymitywnych
	switch (objectType) {
		case PrimitiveObjectType::CUBE:
			return new Cube(attributes, Shaders::getCommonVertexShaderPath(), Shaders::getCommonFragmentShaderPath());
		case PrimitiveObjectType::PLANE:
			return new Plane(attributes, Shaders::getCommonVertexShaderPath(), Shaders::getCommonFragmentShaderPath());
		case PrimitiveObjectType::SPHERE:
			return new Sphere(attributes, Shaders::getCommonVertexShaderPath(), Shaders::getCommonFragmentShaderPath());
		case PrimitiveObjectType::CIRCLE:
			return new Circle(attributes, Shaders::getCommonVertexShaderPath(), Shaders::getCommonFragmentShaderPath());
		case PrimitiveObjectType::CONE:
			return new Cone(attributes, Shaders::getCommonVertexShaderPath(), Shaders::getCommonFragmentShaderPath());
		case PrimitiveObjectType::CYLINDER:
			return new Cylinder(attributes, Shaders::getCommonVertexShaderPath(), Shaders::getCommonFragmentShaderPath());
		case PrimitiveObjectType::TORUS:
			return new Torus(attributes, Shaders::getCommonVertexShaderPath(), Shaders::getCommonFragmentShaderPath());
		default:
			return nullptr;
		}
}
