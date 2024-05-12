#pragma once

#include "Scene/Level/CmLevel.h"

void CmLevel::initBasicObjects() {
	//createObject<PrimitiveObjectFactory, ObjectTypes::PrimitiveObjectType, ObjectAttributes::Common>(
	//	ObjectTypes::PrimitiveObjectType(ObjectTypes::PrimitiveObjectType::CUBE), 
	//	new ObjectAttributes::Common(glm::vec3(0.0f, 0.0f, 35.0f), 
	//		glm::vec3(2.0f), 
	//		glm::vec3(0.0f), 
	//		"Cube"));
	//createObject(ObjectTypes::PrimitiveObjectType::PLANE, new ObjectBasicAttributes(glm::vec3(0.0f, -15.0f, 35.0f), glm::vec3(5.0f), glm::vec3(0.0f), "Plane"));
	//createObject<CubeMarchingObjectFactory, ObjectTypes::CubeMarchingObjectType, ObjectAttributes::CubeMarching::Sphere>(
	//	ObjectTypes::CubeMarchingObjectType(ObjectTypes::CubeMarchingObjectType::SPHERE), 
	//	new ObjectAttributes::CubeMarching::Sphere(
	//		glm::vec3(-40.0f, -15.0f, 35.0f),	// position
	//		glm::vec3(1.0f),					// scale
	//		glm::vec3(0.0f),					// rotation
	//		glm::vec3(40.0f),					// gridSize
	//		1.0f,								// isoValue
	//		5.0f,								// radius
	//		"Sphere (CM)"));

	//createObject<CubeMarchingObjectFactory, ObjectTypes::CubeMarchingObjectType, ObjectAttributes::CubeMarching::Noise>(
	//	ObjectTypes::CubeMarchingObjectType(ObjectTypes::CubeMarchingObjectType::NOISE_3D),
	//	new ObjectAttributes::CubeMarching::Noise(
	//		glm::vec3(0.0f, 25.0f, 25.0f),  //	position
	//		glm::vec3(35.0f),				//	scale
	//		glm::vec3(0.0f),				//	rotation
	//		glm::vec3(40),					//	gridSize
	//		0.05f,							//	isoValue
	//		1.0f,							//	noiseScale
	//		3.0f,							//	frequency
	//		1.0f,							//	amplitude
	//		0.3f,							//	lacunarity
	//		0.15f,							//	persistance
	//		false,							//	LOD
	//		"Noise3d (CM)")); 
	 
	//createObject<CubeMarchingObjectFactory, ObjectTypes::CubeMarchingObjectType, ObjectAttributes::CubeMarching::Noise>(
	//	ObjectTypes::CubeMarchingObjectType(ObjectTypes::CubeMarchingObjectType::NOISE_2D),
	//	new ObjectAttributes::CubeMarching::Noise(
	//		glm::vec3(-400.0f, -40.0f, 50.0f),  //	position
	//		glm::vec3(400, 40, 400),		//	scale
	//		glm::vec3(0.0f),				//	rotation
	//		glm::vec3(60, 35, 60),					//	gridSize
	//		0.05f,							//	isoValue
	//		1.0f,							//	noiseScale
	//		1.1f,							//	frequency
	//		1.0f,							//	amplitude
	//		4.5f,							//	lacunarity
	//		0.7f,							//	persistance
	//		false,							//	LOD
	//		"Noise2d (CM)")); 

	createObject<CubeMarchingObjectFactory, ObjectTypes::CubeMarchingObjectType, ObjectAttributes::CubeMarching::Noise>(
		ObjectTypes::CubeMarchingObjectType(ObjectTypes::CubeMarchingObjectType::NOISE_3D_GEOMETRY),
		new ObjectAttributes::CubeMarching::Noise(
			glm::vec3(0.0f, 25.0f, 25.0f),  //	position
			glm::vec3(35.0f),				//	scale
			glm::vec3(0.0f),				//	rotation
			glm::vec3(70),					//	gridSize
			0.05f,							//	isoValue
			1.0f,							//	noiseScale
			3.0f,							//	frequency
			1.0f,							//	amplitude
			0.3f,							//	lacunarity
			0.15f,							//	persistance
			false,							//	LOD
			"Noise3d (CM)")); 

	//createObject<CubeMarchingObjectFactory, ObjectTypes::CubeMarchingObjectType, ObjectAttributes::CubeMarching::Noise>(
	//	ObjectTypes::CubeMarchingObjectType(ObjectTypes::CubeMarchingObjectType::NOISE_2D_GEOMETRY),
	//	new ObjectAttributes::CubeMarching::Noise(
	//		glm::vec3(0.0f, -40.0f, 50.0f),  //	position
	//		glm::vec3(70, 35, 70),		//	scale
	//		glm::vec3(0.0f),				//	rotation
	//		glm::vec3(150),					//	gridSize
	//		//glm::vec3(50.0f, 60.0f, 50.0f),					//	gridSize
	//		0.05f,							//	isoValue
	//		1.0f,							//	noiseScale
	//		1.0f,							//	frequency
	//		1.0f,							//	amplitude
	//		1.9f,							//	lacunarity
	//		0.4f,							//	persistance
	//		false,							//	LOD
	//		"Noise2d (CM)")); 

	//createObject<CubeMarchingObjectFactory, ObjectTypes::CubeMarchingObjectType, ObjectAttributes::CubeMarching::Noise>(
	//	ObjectTypes::CubeMarchingObjectType(ObjectTypes::CubeMarchingObjectType::SPH_COMPUTE),
	//	new ObjectAttributes::CubeMarching::Noise(
	//		glm::vec3(0.0f, 25.0f, 25.0f),  //	position
	//		glm::vec3(35.0f),				//	scale
	//		glm::vec3(0.0f),				//	rotation
	//		glm::vec3(120),					//	gridSize
	//		1.0f,							//	isoValue
	//		1.0f,							//	noiseScale
	//		3.0f,							//	frequency
	//		1.0f,							//	amplitude
	//		1.0f,							//	lacunarity
	//		0.5f,							//	persistance
	//		false,							//	LOD
	//		"SPH (CM)"));

	createObject<LightObjectFactory, ObjectTypes::LightObjectType, ObjectAttributes::Lights::DirectionalLight>(
		ObjectTypes::LightObjectType(ObjectTypes::LightObjectType::DIRECTION_LIGHT),
		new ObjectAttributes::Lights::DirectionalLight(
			glm::vec3(0.0f, 150.0f, 50.0f),		//	position
			glm::vec3(0.5f),					//	scale
			glm::vec3(0.0f),					//	rotation
			glm::vec3(-0.2f, -1.0f, -0.3f),		//	direction
			glm::vec3(0.02f, 0.15f, 0.05f),		//	ambient
			glm::vec3(0.4f, 0.4f, 0.4f),		//	diffuse
			glm::vec3(0.5f, 0.5f, 0.5f)));		//  specular

	createObject<LightObjectFactory, ObjectTypes::LightObjectType, ObjectAttributes::Lights::PointLight>(
		ObjectTypes::LightObjectType(ObjectTypes::LightObjectType::POINT_LIGHT),
		new ObjectAttributes::Lights::PointLight(
			glm::vec3(15.0f, 15.0f, 7.0f),		//	position
			glm::vec3(0.2f),					//	scale
			glm::vec3(0.0f),					//	rotation
			glm::vec3(0.2f, 0.2f, 0.1f),		//	ambient
			glm::vec3(0.1f, 0.1f, 0.9f),		//	diffuse
			glm::vec3(1.0f, 1.0f, 0.6f),		//	specular
			0.015f,								//  constant
			0.20f,								//  linear
			0.0007f								//  quadratic
		));
	createObject<LightObjectFactory, ObjectTypes::LightObjectType, ObjectAttributes::Lights::PointLight>(
		ObjectTypes::LightObjectType(ObjectTypes::LightObjectType::POINT_LIGHT),
		new ObjectAttributes::Lights::PointLight(
			glm::vec3(15.0f, 15.0f, 42.0),		//	position
			glm::vec3(0.2f),					//	scale
			glm::vec3(0.0f),					//	rotation
			glm::vec3(0.2f, 0.2f, 0.7f),		//	ambient
			glm::vec3(0.9f, 0.1f, 0.1f),		//	diffuse
			glm::vec3(1.0f, 1.0f, 0.6f),		//	specular
			0.015f,								//  constant
			0.20f,								//  linear
			0.0007f								//  quadratic
		));
	createObject<LightObjectFactory, ObjectTypes::LightObjectType, ObjectAttributes::Lights::PointLight>(
		ObjectTypes::LightObjectType(ObjectTypes::LightObjectType::POINT_LIGHT),
		new ObjectAttributes::Lights::PointLight(
			glm::vec3(-15.0f, 15.0f, 42.0f),		//	position
			glm::vec3(0.2f),					//	scale
			glm::vec3(0.0f),					//	rotation
			glm::vec3(0.2f, 0.2f, 0.3f),		//	ambient
			glm::vec3(0.1f, 0.9f, 0.1f),		//	diffuse
			glm::vec3(1.0f, 1.0f, 0.6f),		//	specular
			0.015f,								//  constant
			0.20f,								//  linear
			0.0007f								//  quadratic
		));
	createObject<LightObjectFactory, ObjectTypes::LightObjectType, ObjectAttributes::Lights::PointLight>(
		ObjectTypes::LightObjectType(ObjectTypes::LightObjectType::POINT_LIGHT),
		new ObjectAttributes::Lights::PointLight(
			glm::vec3(-15.0f, 15.0f, 7.0f),		//	position
			glm::vec3(0.2f),					//	scale
			glm::vec3(0.0f),					//	rotation
			glm::vec3(0.2f, 0.2f, 0.2f),		//	ambient
			glm::vec3(0.1f, 0.1f, 0.1f),		//	diffuse
			glm::vec3(1.0f, 1.0f, 0.6f),		//	specular
			0.015f,								//  constant
			0.20f,								//  linear
			0.0007f								//  quadratic
		));

	//createObject<CubeMarchingObjectFactory, ObjectTypes::CubeMarchingObjectType, ObjectAttributes::CubeMarching::Noise>(
	//	ObjectTypes::CubeMarchingObjectType(ObjectTypes::CubeMarchingObjectType::NOISE_3D_COMPUTE), 
	//	new ObjectAttributes::CubeMarching::Noise(
	//		glm::vec3(25.0f, 25.0f, 25.0f),  //	position
	//		glm::vec3(40, 40, 40),			//	scale
	//		glm::vec3(0.0f),				//	rotation
	//		glm::vec3(40,40,40),					//	gridSize
	//		0.05f,							//	isoValue
	//		1.0f,							//	noiseScale
	//		1.1f,							//	frequency
	//		1.0f,							//	amplitude
	//		4.5f,							//	lacunarity
	//		0.7f,							//	persistance
	//		false,							//	LOD
	//		"Noise3d (CM) - compute")); 

}
