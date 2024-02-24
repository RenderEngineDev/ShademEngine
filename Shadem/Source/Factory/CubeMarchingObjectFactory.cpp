#pragma once
#include "CubeMarching/CmObject.h"
#include "Factory/CubeMarchingObjectFactory.h"

using namespace ObjectTypes;

Object* CubeMarchingObjectFactory::createObject(CubeMarchingObjectType::Type objectType, ObjectAttributes::Common* attributes) {
	switch (objectType) {
		case ObjectTypes::CubeMarchingObjectType::CUBE:
			// TODO:
			return nullptr;
		case ObjectTypes::CubeMarchingObjectType::PLANE:
			return nullptr;
		case ObjectTypes::CubeMarchingObjectType::SPHERE:
			return new CubeMarching::Sphere(dynamic_cast<ObjectAttributes::CubeMarching*>(attributes), Shaders::getCommonVertexShaderPath(), Shaders::getCommonFragmentShaderPath());
		case ObjectTypes::CubeMarchingObjectType::RANDOM:
			return new CubeMarching::Random(dynamic_cast<ObjectAttributes::CubeMarching*>(attributes), Shaders::getCommonVertexShaderPath(), Shaders::getCommonFragmentShaderPath());
		case ObjectTypes::CubeMarchingObjectType::NOISE:
			return new CubeMarching::Noise(dynamic_cast<ObjectAttributes::CubeMarching*>(attributes), Shaders::getCommonVertexShaderPath(), Shaders::getCommonFragmentShaderPath());
		default:
			return nullptr;
	}
}
