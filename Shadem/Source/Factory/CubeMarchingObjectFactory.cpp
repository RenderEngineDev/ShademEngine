#pragma once
#include "CubeMarching/CmObject.h"
#include "Factory/CubeMarchingObjectFactory.h"

#define CM_ATTRIBUTE ObjectAttributes::CubeMarching
#define OBJECT_TYPE ObjectTypes::CubeMarchingObjectType
#define FACTORY CubeMarchingObjectFactory

using namespace Shaders;
using namespace CubeMarching;

Object* CubeMarchingObjectFactory::createObject(OBJECT_TYPE::Type objectType, ObjectAttributes::Common* attributes) {
	if (auto castedAttribute = FACTORY::castTo<CM_ATTRIBUTE::Sphere>(attributes)) {
		return createSphericalObject(objectType, castedAttribute);
	}

	if (auto castedAttribute = FACTORY::castTo<CM_ATTRIBUTE::Noise>(attributes)) {
		return createNoiseObject(objectType, castedAttribute);
	};

	if (auto castedAttribute = FACTORY::castTo<CM_ATTRIBUTE::CommonCubeMarching>(attributes)) {
		return createCommonObject(objectType, castedAttribute);
	};
}

Object* CubeMarchingObjectFactory::createNoiseObject(OBJECT_TYPE::Type objectType, CM_ATTRIBUTE::Noise* castedAttribute) {
	switch (objectType) {
	case OBJECT_TYPE::NOISE_2D: {
		if (castedAttribute->lodOn)
			return new Noise2D(castedAttribute, { getCMVertexShaderPath(), getCommonFragmentShaderPath(), getTesselationTCSShaderPath(), getTesselationTESShaderPath() });
		else
			return new Noise2D(castedAttribute, { getCMVertexShaderPath(), getCommonFragmentShaderPath() });
	}
	case OBJECT_TYPE::NOISE_3D: {
		if (castedAttribute->lodOn)
			return new Noise3D(castedAttribute, { getCMVertexShaderPath(), getCommonFragmentShaderPath(), getTesselationTCSShaderPath(), getTesselationTESShaderPath() });
		else
			return new Noise3D(castedAttribute, { getCMVertexShaderPath(), getCommonFragmentShaderPath() });
	}
	case OBJECT_TYPE::NOISE_2D_GEOMETRY:
		return new NoiseGeometry2D(castedAttribute, { getCMVertexShaderPath(), getCMFragmentShaderPath(), getGeometryShaderCM2DPath() });
	case OBJECT_TYPE::NOISE_3D_GEOMETRY:
		return new NoiseGeometry3D(castedAttribute, { getCMVertexShaderPath(), getCMFragmentShaderPath(), getGeometryShaderCM3DPath() });
	case OBJECT_TYPE::NOISE_3D_COMPUTE:
		return new Noise3DCompute(castedAttribute, { getComputeShaderPath() });
	case OBJECT_TYPE::SPH_COMPUTE:
		return new SPHSystem(10, glm::vec3(0.0f), glm::vec3(100.0f), castedAttribute, { getCMVertexShaderPath(), getCMFragmentShaderPath(), getGeometryShaderCM3DPath() });
	default:
		return nullptr;
	}
}

Object* CubeMarchingObjectFactory::createSphericalObject(OBJECT_TYPE::Type objectType, CM_ATTRIBUTE::Sphere* castedAttribute) {
	switch (objectType) {
	case OBJECT_TYPE::SPHERE:
		return new Sphere(castedAttribute, { getCMVertexShaderPath(), getCMFragmentShaderPath() });
	default:
		return nullptr;
	}
}

Object* CubeMarchingObjectFactory::createCommonObject(OBJECT_TYPE::Type objectType, CM_ATTRIBUTE::CommonCubeMarching* castedAttribute) {
	switch (objectType) {
	case OBJECT_TYPE::CUBE:
		// TODO:
		return nullptr;
	case OBJECT_TYPE::PLANE:
		return nullptr;
	case OBJECT_TYPE::RANDOM: {
		return new Random(castedAttribute,
			{ getCMVertexShaderPath(), getCommonFragmentShaderPath() });
	}
	default:
		return nullptr;
	}
}

template<class DestinateType>
DestinateType* CubeMarchingObjectFactory::castTo(ObjectAttributes::Common* attributes) {
	return dynamic_cast<DestinateType*>(attributes);
}