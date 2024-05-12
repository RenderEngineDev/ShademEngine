#pragma once

#include "CubeMarching/CmObject.h"
#include "Objects/Lights/Light.h"

using namespace CubeMarching;

#include "ShademApplication/ShademEngine.h"

Noise::Noise(ObjectAttributes::CubeMarching::Noise* attributes, std::initializer_list<const std::string> filePaths) : attributes(attributes), CmObject(attributes, filePaths) {	// TODO: do metody wrzuciæ
	/*if (attributes->lodOn) {
		glPatchParameteri(GL_PATCH_VERTICES, 3);
	}
	setupMesh();*/
}

Noise2D::Noise2D(ObjectAttributes::CubeMarching::Noise* attributes, std::initializer_list<const std::string> filePaths) : Noise(attributes, filePaths) {
	if (attributes->lodOn) {
		glPatchParameteri(GL_PATCH_VERTICES, 3);
	}
	setupMesh();
}

Noise3D::Noise3D(ObjectAttributes::CubeMarching::Noise* attributes, std::initializer_list<const std::string> filePaths) : Noise(attributes, filePaths) {
	if (attributes->lodOn) {
		glPatchParameteri(GL_PATCH_VERTICES, 3);
	}
	setupMesh();
}

// TODO: swapowanie shaderów 
NoiseGeometry::NoiseGeometry(ObjectAttributes::CubeMarching::Noise* attributes, std::initializer_list<const std::string> filePaths) : Noise(attributes, filePaths) {
}
NoiseGeometry2D::NoiseGeometry2D(ObjectAttributes::CubeMarching::Noise* attributes, std::initializer_list<const std::string> filePaths) : NoiseGeometry(attributes, filePaths) {
	setupMesh();
}
NoiseGeometry3D::NoiseGeometry3D(ObjectAttributes::CubeMarching::Noise* attributes, std::initializer_list<const std::string> filePaths) : NoiseGeometry(attributes, filePaths) {
	//ShademEngine::start = std::chrono::steady_clock::now();
	setupMesh();
}


Noise3DCompute::Noise3DCompute(ObjectAttributes::CubeMarching::Noise* attributes, std::initializer_list<const std::string> filePaths) : Noise(attributes, filePaths) {
	// texture size
	const unsigned int TEXTURE_WIDTH = 512, TEXTURE_HEIGHT = 512;

	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);

	//glBindImageTexture(0, texture, 0, GL_FALSE, 0, GL_READ, GL_RGBA32F);
	glBindImageTexture(0, texture, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);

	rectangleShader = new Shader({Shaders::getComputeVertexShaderPath(), Shaders::getComputeFragmentShaderPath()});
		
	setupMesh();
}

void Noise::draw(Camera::Camera& camera) {
	shader->use();
	glm::mat4 model = glm::mat4(1.0f);
	evaluateBasicModelTransformations(model);
	
	shader->setMat4("MVP", camera.getProjection() * camera.getView() * model);
	
	shader->setVec3("gridSize", attributes->gridSize);
	shader->setVec3("offset", attributes->offset);
	shader->setFloat("noiseScale", attributes->noiseScale);
	shader->setFloat("isoValue", attributes->isoValue);
	shader->setFloat("frequency", attributes->frequency);
	shader->setFloat("amplitude", attributes->amplitude);
	shader->setFloat("persistence", attributes->persistence);
	shader->setFloat("lacunarity", attributes->lacunarity);
	Lights::Light::setShaderLights(shader.get());

	shader->setVec3("cameraPosition", camera.position);
	for (auto& mesh : *meshes) {
		if (attributes->lodOn) {
			mesh->DrawPatches();
		}
		else {
			mesh->DrawWithoutIndices(shader);
		}
	}
}

void NoiseGeometry::draw(Camera::Camera& camera) {
	shader->use();
	glm::mat4 model = glm::mat4(1.0f);
	evaluateBasicModelTransformations(model);
	shader->setMat4("model", model);
	shader->setMat4("MVP", camera.getProjection() * camera.getView() * model);
	
	shader->setVec3("gridSize", attributes->gridSize);
	shader->setVec3("offset", attributes->offset);
	shader->setFloat("noiseScale", attributes->noiseScale);
	shader->setFloat("isoValue", attributes->isoValue);
	shader->setFloat("frequency", attributes->frequency);
	shader->setFloat("amplitude", attributes->amplitude);
	shader->setFloat("persistence", attributes->persistence);
	shader->setFloat("lacunarity", attributes->lacunarity);

	Lights::Light::setShaderLights(shader.get());

	shader->setVec3("cameraPosition", camera.position);

	for (auto& mesh : *meshes) {
		mesh->DrawGeoPointsCM();
	}
}

void Noise3DCompute::draw(Camera::Camera& camera) {
	shader->use();
	glm::mat4 model = glm::mat4(1.0f);
	evaluateBasicModelTransformations(model);
	shader->setMat4("MVP", camera.getProjection() * camera.getView() * model);
	const unsigned int TEXTURE_WIDTH = 512, TEXTURE_HEIGHT = 512;
	shader->dispatch(TEXTURE_WIDTH, TEXTURE_HEIGHT, 1);

	// render image to quad
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	rectangleShader->use();
	rectangleShader->setInt("tex", 255);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	for (auto& mesh : *meshes) {
		mesh->DrawRectangleForComputeShader();
	}
}

void Noise::update(Camera::Camera& camera) {
}

void Noise2D::update(Camera::Camera& camera) {
	if (isUpdated()) {
		trianglesGenerator->triangulate_field(gridGenerator->generate_noise2D(getAttributes()->frequency, getAttributes()->amplitude, getAttributes()->lacunarity, getAttributes()->persistence, getAttributes()->noiseScale, getAttributes()->offset, scalarFunction), getAttributes()->isoValue);
		(*meshes)[0]->vertices = convertTrianglesToVertices(trianglesGenerator->triangles);

		glBindBuffer(GL_ARRAY_BUFFER, (*meshes)[0]->getVBO());
		glBufferData(GL_ARRAY_BUFFER, (*meshes)[0]->vertices.size() * sizeof(Vertex), &(*meshes)[0]->vertices[0], GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		setUpdated(false);
	}
}

void Noise3D::update(Camera::Camera& camera) {
	if (isUpdated()) {
		trianglesGenerator->triangulate_field(gridGenerator->generate_noise3D(getAttributes()->frequency, getAttributes()->amplitude, getAttributes()->lacunarity, getAttributes()->persistence, getAttributes()->noiseScale, getAttributes()->offset, scalarFunction), getAttributes()->isoValue);
		(*meshes)[0]->vertices = convertTrianglesToVertices(trianglesGenerator->triangles);

		glBindBuffer(GL_ARRAY_BUFFER, (*meshes)[0]->getVBO());
		glBufferData(GL_ARRAY_BUFFER, (*meshes)[0]->vertices.size() * sizeof(Vertex), &(*meshes)[0]->vertices[0], GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		setUpdated(false);
	}
}

void NoiseGeometry::update(Camera::Camera& camera) {
}

void NoiseGeometry3D::update(Camera::Camera& camera) {
	if (isUpdated()) {
		//scalarFunction = std::vector<std::vector<std::vector<float>>>(attributes->gridSize.x, std::vector<std::vector<float>>(attributes->gridSize.y, std::vector<float>(attributes->gridSize.z)));
		//gridGenerator->generate_noise3D(getAttributes()->frequency, getAttributes()->amplitude, getAttributes()->lacunarity, getAttributes()->persistence, getAttributes()->noiseScale, getAttributes()->offset, scalarFunction);
		gridGenerator->generate_noise3D(getAttributes()->frequency, getAttributes()->amplitude, getAttributes()->lacunarity, getAttributes()->persistence, getAttributes()->noiseScale, getAttributes()->offset, scalarFunction);
		(*meshes)[0]->vertices = calculateCellsFirstVertices();
		//attributes->frequency = (glm::sin(ShademEngine::ttttime++ / 10 * 3.14f / 180.0f) + 1.0f) * 7.0f;
		//(*meshes)[0]->setupMeshWithouIndicesTemp();
		//bindCellsValuesVBO();
		glBindBuffer(GL_ARRAY_BUFFER, (*meshes)[0]->getVBO());
		glBufferData(GL_ARRAY_BUFFER, (*meshes)[0]->vertices.size() * sizeof(Vertex), &(*meshes)[0]->vertices[0], GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ARRAY_BUFFER, cellValuesVBO);
		glBufferData(GL_ARRAY_BUFFER, cellsValues.size() * sizeof(GridCellValues), &cellsValues[0], GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		setUpdated(false);
	}
}

void NoiseGeometry2D::update(Camera::Camera& camera) {
	if (isUpdated()) {
		//scalarFunction = std::vector<std::vector<std::vector<float>>>(attributes->gridSize.x, std::vector<std::vector<float>>(attributes->gridSize.y, std::vector<float>(attributes->gridSize.z)));
		//gridGenerator->generate_noise3D(getAttributes()->frequency, getAttributes()->amplitude, getAttributes()->lacunarity, getAttributes()->persistence, getAttributes()->noiseScale, getAttributes()->offset, scalarFunction);
		gridGenerator->generate_noise2D(getAttributes()->frequency, getAttributes()->amplitude, getAttributes()->lacunarity, getAttributes()->persistence, getAttributes()->noiseScale, getAttributes()->offset, scalarFunction);
		(*meshes)[0]->vertices = calculateCellsFirstVertices();
		//(*meshes)[0]->setupMeshWithouIndicesTemp();
		//bindCellsValuesVBO();
		glBindBuffer(GL_ARRAY_BUFFER, (*meshes)[0]->getVBO());
		glBufferData(GL_ARRAY_BUFFER, (*meshes)[0]->vertices.size() * sizeof(Vertex), &(*meshes)[0]->vertices[0], GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ARRAY_BUFFER, cellValuesVBO);
		glBufferData(GL_ARRAY_BUFFER, cellsValues.size() * sizeof(GridCellValues), &cellsValues[0], GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		setUpdated(false);
	}
}

void CubeMarching::NoiseGeometry::bindCellsValuesVBO()
{
	glBindBuffer(GL_ARRAY_BUFFER, cellValuesVBO);
	glBufferData(GL_ARRAY_BUFFER, cellsValues.size() * sizeof(GridCellValues), &cellsValues[0], GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(GridCellValues), (void*)0);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(GridCellValues), (void*)offsetof(GridCellValues, valuesR));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


void Noise::setupMesh() {
	//initGenerators();
	//mesh = new Mesh(calculateCellsFirstVertices(), std::vector<std::shared_ptr<Texture>>{});
}

void Noise2D::setupMesh() {
	initGenerators();
	trianglesGenerator->triangulate_field(gridGenerator->generate_noise2D(getAttributes()->frequency, getAttributes()->amplitude, getAttributes()->lacunarity, getAttributes()->persistence, getAttributes()->noiseScale, getAttributes()->offset, scalarFunction), getAttributes()->isoValue);
	meshes = std::make_shared<std::vector<Mesh*>>(std::vector<Mesh*>{new Mesh(convertTrianglesToVertices(trianglesGenerator->triangles), std::vector<std::shared_ptr<Texture>>{})});
}

void Noise3D::setupMesh() {
	initGenerators();
	trianglesGenerator->triangulate_field(gridGenerator->generate_noise3D(getAttributes()->frequency, getAttributes()->amplitude, getAttributes()->lacunarity, getAttributes()->persistence, getAttributes()->noiseScale, getAttributes()->offset, scalarFunction), getAttributes()->isoValue);
	meshes = std::make_shared<std::vector<Mesh*>>(std::vector<Mesh*>{new Mesh(convertTrianglesToVertices(trianglesGenerator->triangles), std::vector<std::shared_ptr<Texture>>{})});
}

void NoiseGeometry2D::setupMesh() {
	initGenerators();
	//gridGenerator->generate_noise3D(getAttributes()->frequency, getAttributes()->amplitude, getAttributes()->lacunarity, getAttributes()->persistence, getAttributes()->noiseScale, getAttributes()->offset, scalarFunction);
	gridGenerator->generate_noise2D(getAttributes()->frequency, getAttributes()->amplitude, getAttributes()->lacunarity, getAttributes()->persistence, getAttributes()->noiseScale, getAttributes()->offset, scalarFunction);
	//meshes = std::make_shared<std::vector<Mesh*>>(std::vector<Mesh*>{new Mesh(calculateCellsFirstVertices(), std::vector<std::shared_ptr<Texture>>{})});

	glGenBuffers(1, &cellValuesVBO);
	std::vector<Vertex> vertices = calculateCellsFirstVertices();

	meshes = std::make_shared<std::vector<Mesh*>>(std::vector<Mesh*>{new Mesh(vertices)});
	(*meshes)[0]->setupMeshWithouIndicesTemp();
	bindCellsValuesVBO();

}

void NoiseGeometry3D::setupMesh() {
	initGenerators();
	//gridGenerator->generate_noise3D(getAttributes()->frequency, getAttributes()->amplitude, getAttributes()->lacunarity, getAttributes()->persistence, getAttributes()->noiseScale, getAttributes()->offset, scalarFunction);
	gridGenerator->generate_noise3D(getAttributes()->frequency, getAttributes()->amplitude, getAttributes()->lacunarity, getAttributes()->persistence, getAttributes()->noiseScale, getAttributes()->offset, scalarFunction);
	//meshes = std::make_shared<std::vector<Mesh*>>(std::vector<Mesh*>{new Mesh(calculateCellsFirstVertices(), std::vector<std::shared_ptr<Texture>>{})});

	glGenBuffers(1, &cellValuesVBO);
	std::vector<Vertex> vertices = calculateCellsFirstVertices();

	meshes = std::make_shared<std::vector<Mesh*>>(std::vector<Mesh*>{new Mesh(vertices)});
	(*meshes)[0]->setupMeshWithouIndicesTemp();
	bindCellsValuesVBO();

}

void Noise3DCompute::setupMesh() {
	std::vector<Vertex> vertices{
		Vertex(glm::vec3(-1.0f,  1.0f, 0.0f), glm::vec2(0.0f, 1.0f)),
		Vertex(glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f)),
		Vertex(glm::vec3(1.0f,  1.0f, 0.0f), glm::vec2(1.0f, 1.0f)),
		Vertex(glm::vec3(1.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f))
	};
	meshes = std::make_shared<std::vector<Mesh*>>(std::vector<Mesh*>{new Mesh(vertices, std::vector<std::shared_ptr<Texture>>{})});
}

std::vector<Vertex> Noise::calculateCellsFirstVertices() {
	std::vector<Vertex> vertices;
	cellsValues.clear();
	glm::ivec3 igridSize = attributes->gridSize;
	for (int i = 0; i < (igridSize.x - 1); i++) {
		for (int j = 0; j < (igridSize.y - 1); j++) {
			for (int k = 0; k < (igridSize.z - 1); k++) {
				float	x = i / attributes->gridSize.x - 0.5f, 
						y = j / attributes->gridSize.y - 0.5f, 
						z = k / attributes->gridSize.z - 0.5f;
				int cubeIndex = 0;
				cubeIndex |= ((scalarFunction[i][j][k] < attributes->isoValue ? 1 : 0) << 0);
				cubeIndex |= ((scalarFunction[i+1][j][k] < attributes->isoValue ? 1 : 0) << 1);
				cubeIndex |= ((scalarFunction[i+1][j][k+1] < attributes->isoValue ? 1 : 0) << 2);
				cubeIndex |= ((scalarFunction[i][j][k+1] < attributes->isoValue ? 1 : 0) << 3);
				cubeIndex |= ((scalarFunction[i][j+1][k] < attributes->isoValue ? 1 : 0) << 4);
				cubeIndex |= ((scalarFunction[i+1][j+1][k] < attributes->isoValue ? 1 : 0) << 5);
				cubeIndex |= ((scalarFunction[i+1][j+1][k+1] < attributes->isoValue ? 1 : 0) << 6);
				cubeIndex |= ((scalarFunction[i][j+1][k+1] < attributes->isoValue ? 1 : 0) << 7);

				if ((cubeIndex != 0) && (cubeIndex != 255)) {
					vertices.emplace_back(Vertex(glm::vec3(x, y, z)));
					cellsValues.push_back({
						glm::vec4(scalarFunction[i][j][k],
							scalarFunction[i + 1][j][k],
							scalarFunction[i + 1][j][k + 1],
							scalarFunction[i][j][k + 1]),
						glm::vec4(scalarFunction[i][j + 1][k],
							scalarFunction[i + 1][j + 1][k],
							scalarFunction[i + 1][j + 1][k + 1],
							scalarFunction[i][j + 1][k + 1])
						});
				}
				//vertices.emplace_back(Vertex(glm::vec3(x, y, z)));
			}
		}
	};
	return vertices;
}

std::vector<Vertex> Noise::convertTrianglesToVertices(std::vector<std::vector<Point>> triangles) {
	size_t totalSize = 0;
	for (const auto& triangle : triangles) {
		totalSize += triangle.size();
	}
	std::vector<Vertex> flatVector;
	flatVector.reserve(totalSize);
	for (const auto& triangle : triangles) {
		for (const auto& vertex : triangle) {
			flatVector.emplace_back(Vertex(glm::vec3(vertex.x, vertex.y, vertex.z), calculateFaceNormal(triangle)));
			//flatVector.emplace_back(Vertex(glm::vec3(vertex.x, vertex.y, vertex.z), calculateFaceNormal(triangle)));
		}
	}
	return flatVector;
}

void CubeMarching::Noise::initGenerators() {
	glm::vec3& noiseGridSize = attributes->gridSize;
	scalarFunction = std::vector<std::vector<std::vector<float>>>(noiseGridSize.x, std::vector<std::vector<float>>(noiseGridSize.y, std::vector<float>(noiseGridSize.z)));

	trianglesGenerator = new TrianglesGenerator(noiseGridSize);
	gridGenerator = new GridGenerator(noiseGridSize);
}

glm::vec3 Noise::calculateFaceNormal(const std::vector<Point> &triangle) {
	return glm::normalize(glm::cross(glm::vec3(triangle[1].x - triangle[0].x, triangle[1].y - triangle[0].y, triangle[1].z - triangle[0].z),
		glm::vec3(triangle[2].x - triangle[0].x, triangle[2].y - triangle[0].y, triangle[2].z - triangle[0].z)));
}