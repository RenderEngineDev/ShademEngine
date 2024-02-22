#pragma once
#include "Factory/LightObjectFactory.h"
#include "Objects/Lights/Light.h"
#include "Shader/Shader.h"

using namespace Shaders;

Object* LightObjectFactory::createObject(ObjectTypes::LightObjectType::Type objectType, ObjectAttributes::Common* attributes) {
	switch (objectType) {
		case ObjectTypes::LightObjectType::POINT_LIGHT: {
			return new Lights::PointLight(dynamic_cast<ObjectAttributes::Lights::PointLight*>(attributes), ObjectTypes::LightObjectType::POINT_LIGHT, {getLightVertexShaderPath(), getLightFragmentShaderPath()});
		case ObjectTypes::LightObjectType::DIRECTION_LIGHT:
			return new Lights::DirectionalLight(dynamic_cast<ObjectAttributes::Lights::DirectionalLight*>(attributes), ObjectTypes::LightObjectType::DIRECTION_LIGHT, { getLightVertexShaderPath(), getLightFragmentShaderPath() });
		case ObjectTypes::LightObjectType::POSITION_LIGHT:
			return new Lights::PositionLight(dynamic_cast<ObjectAttributes::Lights::PositionLight*>(attributes), ObjectTypes::LightObjectType::POSITION_LIGHT, { getLightVertexShaderPath(), getLightFragmentShaderPath() });
		case ObjectTypes::LightObjectType::SPOT_LIGHT:
			return new Lights::SpotLight(dynamic_cast<ObjectAttributes::Lights::SpotLight*>(attributes), ObjectTypes::LightObjectType::SPOT_LIGHT, { getLightVertexShaderPath(), getLightFragmentShaderPath() });
		default:
			return nullptr;
		}
	}
}