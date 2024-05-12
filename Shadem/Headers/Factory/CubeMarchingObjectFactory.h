#pragma once

#include "Objects/Object.h"
#include "Objects/ObjectTypes.h"
#include "Objects/Attributes.h"

class CubeMarchingObjectFactory {
public:
	static Object* createObject(ObjectTypes::CubeMarchingObjectType::Type objectType, ObjectAttributes::Common* attributes);


private:
	static Object* createNoiseObject(ObjectTypes::CubeMarchingObjectType::Type objectType, ObjectAttributes::CubeMarching::Noise* castedAttribute);
	static Object* createSphericalObject(ObjectTypes::CubeMarchingObjectType::Type objectType, ObjectAttributes::CubeMarching::Sphere* castedAttribute);
	static Object* createCommonObject(ObjectTypes::CubeMarchingObjectType::Type objectType, ObjectAttributes::CubeMarching::CommonCubeMarching* castedAttribute);
	
	template<class DestinateType>
	static DestinateType* castTo(ObjectAttributes::Common* attributes);
};