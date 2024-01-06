#pragma once
#include "CubeMarching/CmObject.h"
#include "Factory/CubeMarchingObjectFactory.h"

using namespace ObjectTypes;

Object* CubeMarchingObjectFactory::createObject(CubeMarchingObjectType::Type objectType, CubeMarchingAttributes attributes) {
	//CubeMarchingAttributes* cmAttributes = dynamic_cast<CubeMarchingAttributes*>(attributes);

	switch (objectType) {
		case ObjectTypes::CubeMarchingObjectType::CUBE:
			// TODO:
			return nullptr;
		case ObjectTypes::CubeMarchingObjectType::PLANE:
			// TODO:
			return nullptr;
		case ObjectTypes::CubeMarchingObjectType::SPHERE:
			return new CubeMarching::Sphere(attributes.gridSize, attributes.isoValue, new Shader(Shaders::getCommonVertexShaderPath(), Shaders::getCommonFragmentShaderPath()), attributes.position, attributes.scale, attributes.radius);
		case ObjectTypes::CubeMarchingObjectType::RANDOM:
			return new CubeMarching::Random(attributes.gridSize, attributes.isoValue, new Shader(Shaders::getCommonVertexShaderPath(), Shaders::getCommonFragmentShaderPath()), attributes.position, attributes.scale);
		default:
			return nullptr;
	}
}
