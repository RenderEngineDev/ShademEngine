#pragma once
#include "CubeMarching/CmObject.h"
#include "Factory/CubeMarchingObjectFactory.h"

using namespace ObjectTypes;
using namespace Shaders;

Object* CubeMarchingObjectFactory::createObject(CubeMarchingObjectType::Type objectType, ObjectAttributes::Common* attributes) {
	if (auto castedAttribute = dynamic_cast<ObjectAttributes::CubeMarching::Sphere*>(attributes)) {
		switch (objectType) {
			case ObjectTypes::CubeMarchingObjectType::SPHERE: {
				return new CubeMarching::Sphere(castedAttribute,
					{ getCMVertexShaderPath(), getCMFragmentShaderPath() });
			}
			default:
				return nullptr;
		}
	}

	if (auto castedAttribute = dynamic_cast<ObjectAttributes::CubeMarching::Noise*>(attributes)) {
		switch (objectType) {
			case ObjectTypes::CubeMarchingObjectType::NOISE_2D: {
				if (castedAttribute->lodOn)
					return new CubeMarching::Noise2D(castedAttribute,
						{ getCMVertexShaderPath(), getCommonFragmentShaderPath(), getTesselationTCSShaderPath(), getTesselationTESShaderPath() });
				else
					return new CubeMarching::Noise2D(castedAttribute,
						{ getCMVertexShaderPath(), getCommonFragmentShaderPath() });
			}
			case ObjectTypes::CubeMarchingObjectType::NOISE_3D: {
				if (castedAttribute->lodOn)
					return new CubeMarching::Noise3D(castedAttribute,
						{ getCMVertexShaderPath(), getCommonFragmentShaderPath(), getTesselationTCSShaderPath(), getTesselationTESShaderPath() });
				else
					return new CubeMarching::Noise3D(castedAttribute,
						{ getCMVertexShaderPath(), getCommonFragmentShaderPath() });
			}
			case ObjectTypes::CubeMarchingObjectType::NOISE_2D_GEOMETRY: {
				return new CubeMarching::NoiseGeometry(castedAttribute,
					{ getCMVertexShaderPath(), getCMFragmentShaderPath(), getGeometryShaderCM2DPath() });
			}
			case ObjectTypes::CubeMarchingObjectType::NOISE_3D_GEOMETRY: {
				return new CubeMarching::NoiseGeometry(castedAttribute,
					{ getCMVertexShaderPath(), getCMFragmentShaderPath(), getGeometryShaderCM3DPath() });
			}
			case ObjectTypes::CubeMarchingObjectType::NOISE_3D_COMPUTE: {
				return new CubeMarching::Noise3DCompute(castedAttribute,
					{ getComputeShaderPath() });
			}
			default:
				return nullptr;
		}

		if (auto castedAttribute = dynamic_cast<ObjectAttributes::CubeMarching::CommonCubeMarching*>(attributes)) {
			switch (objectType) {
			case ObjectTypes::CubeMarchingObjectType::CUBE:
				// TODO:
				return nullptr;
			case ObjectTypes::CubeMarchingObjectType::PLANE:
				return nullptr;
			case ObjectTypes::CubeMarchingObjectType::RANDOM: {
				return new CubeMarching::Random(castedAttribute,
					{ getCMVertexShaderPath(), getCommonFragmentShaderPath() });
			}
			default:
				return nullptr;
			}
		}
	};
}