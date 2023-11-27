#pragma once
#include "CubeMarching/CmObject.h"
#include "Factory/CubeMarchingObjectFactory.h"

using namespace ObjectTypes;

Object* CubeMarchingObjectFactory::createObject(CubeMarchingObjectType::Type objectType, ObjectBasicAttributes attributes) {
	switch (objectType) {
		case ObjectTypes::CubeMarchingObjectType::CUBE:
			// TODO:
			return nullptr;
		case ObjectTypes::CubeMarchingObjectType::PLANE:
			// TODO:
			return nullptr;
		case ObjectTypes::CubeMarchingObjectType::SPHERE:
			return new CmObject(MarchingCubeGenerator::StructureType::SPHERE, 80, 1.0f, attributes.position, attributes.radius);
		case ObjectTypes::CubeMarchingObjectType::RANDOM:
			return new CmObject(MarchingCubeGenerator::StructureType::RANDOM, 50, 0.6f, attributes.position, attributes.radius);
		default:
			return nullptr;
	}
}
