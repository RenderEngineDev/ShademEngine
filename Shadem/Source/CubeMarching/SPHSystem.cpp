#pragma once

#include <algorithm> 

#include "CubeMarching/CmObject.h"
#include "Objects/Lights/Light.h"
#include "ShademApplication/ShademEngine.h"
using namespace CubeMarching;

void checkGLError(const std::string& functionName) {
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		std::cerr << "OpenGL error in " << functionName << ": " << std::hex << err << std::endl;
	}
}

SPHSystem::SPHSystem(int numParticles, glm::vec3 minBoundary, glm::vec3 maxBoundary, ObjectAttributes::CubeMarching::Noise* attributes, std::initializer_list<const std::string> filePaths)
	: numParticles(numParticles), minBoundary(minBoundary), maxBoundary(maxBoundary), attributes(attributes), CmObject(attributes, filePaths) {

	computeShader = new Shader({Shaders::getSphSystemComputeShaderPath()});
	computeShaderForScalars = new Shader({Shaders::getSphScalarsComputeShaderPath()});
	computeShaderForNewScalars = new Shader({Shaders::getSphNewScalarsComputeShaderPath()});

	densityShader = new Shader({ Shaders::getSphDensityComputeShaderPath() });
	forcesShader = new Shader({ Shaders::getSphForcesComputeShaderPath() });
	particlesShader = new Shader({ Shaders::getSphParticlesComputeShaderPath() });

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> disX(minBoundary.x + attributes->gridSize.x * 0.05f, attributes->gridSize.x - attributes->gridSize.x * 0.05f);
	std::uniform_real_distribution<float> disY(minBoundary.y + attributes->gridSize.y * 0.05f, attributes->gridSize.y - attributes->gridSize.y * 0.05f);
	std::uniform_real_distribution<float> disZ(minBoundary.z + attributes->gridSize.z * 0.05f, attributes->gridSize.z - attributes->gridSize.z * 0.05f);

	//int n = numParticles * numParticles * numParticles;

	setUniforms(particlesShader);
	setUniforms(forcesShader);
	setUniforms(densityShader);
	setUniforms(computeShaderForNewScalars);

	particles.resize(N3);
	vx_cpu.resize(N3);
	vy_cpu.resize(N3);
	vz_cpu.resize(N3);
	force_pres_x_cpu.resize(N3);
	force_pres_y_cpu.resize(N3);
	force_pres_z_cpu.resize(N3);
	force_visc_x_cpu.resize(N3);
	force_visc_y_cpu.resize(N3);
	force_visc_z_cpu.resize(N3);
	rho_cpu.resize(N3);
	mass_cpu.resize(N3);

	int k = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			for (int m = 0; m < n; m++)
			{
				particles[k].x = 0.25f + 0.25f * i / float(n); //dis(gen32); //0.25 + 0.25 * i / float(n);//ofRandom(0,W)/float(W);
				particles[k].y = 0.25f + 0.25f * j / float(n); //dis(gen32); //0.25 + 0.25 * j / float(n);//ofRandom(0,H)/float(H);
				particles[k].z = 0.25f + 0.25f * m / float(n); //dis(gen32); //0.25 + 0.25 * m / float(n);//1-2*1*0.5*w/float(N3);
				particles[k].w = 1;
				vx_cpu[k] = vy_cpu[k] = vz_cpu[k] = 0;
				mass_cpu[k] = mass0;
				k++;
			}
		}
	}
	setupMesh();
}

void SPHSystem::draw(Camera::Camera& camera) {

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
void SPHSystem::setUniforms(Shader* shader) {
	shader->use();
	float h = 0.035f;// 0.03f;
	float de = 0.02f;
	float rho0 = 5.0f;
	//float mass0 = 0.003;
	float mu = 10.0f;

	float gz = 0.0f;//0.3;
	float gy = GZ;//0.3;
	float gx = 0.0f;
	float dt = 0.007f;
	float k = 8.0f;                 // J/(mol*K) - gas constant
	float M_PI = 3.14159265359f;

	shader->setInt("N", N3);
	shader->setFloat("de", de);
	shader->setFloat("h", h);
	shader->setFloat("powh9", h * h * h * h * h * h * h * h * h);
	shader->setFloat("powh6", h * h * h * h * h * h);
	shader->setFloat("powh2", h * h);
	shader->setFloat("powh5", h * h * h * h * h);
	shader->setFloat("rho0", rho0);
	shader->setFloat("mass0", mass0);
	shader->setFloat("mu", mu);
	shader->setFloat("gz", gz);
	shader->setFloat("gy", gy);
	shader->setFloat("gx", gx);
	shader->setFloat("dt", dt);
	shader->setFloat("k", k);
	shader->setFloat("M_PI", M_PI);
	shader->setFloat("particleRadius", particleRadius);
	shader->setVec3("gridSize", attributes->gridSize);
}

void SPHSystem::update(Camera::Camera& camera) {

	int numGroups = (scalarFunctionFlatted.size() + 127) / 128;
	computeShaderForScalars->use();
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, scalarFunctionBuffer);
	computeShaderForScalars->dispatch(numGroups, 1, 1);
	//glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	uploadParticlesToGPU();
	int pNumGroups = (N3 + 127) / 128;
	densityShader->use();
	densityShader->dispatch(pNumGroups, 1, 1);
	forcesShader->use();
	forcesShader->dispatch(pNumGroups, 1, 1);
	particlesShader->use();
	t += 0.055f;
	particlesShader->setFloat("gz", -2 * GZ * sin(t * 0.1f));
	particlesShader->dispatch(pNumGroups, 1, 1);


	computeShaderForNewScalars->use();
	//glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, p);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, scalarFunctionBuffer);
	setUniforms(computeShaderForNewScalars);
	computeShaderForNewScalars->dispatch(numGroups, 1, 1);

	downloadParticlesFromGPU();

	(*meshes)[0]->vertices = calculateCellsFirstVertices();
	glBindBuffer(GL_ARRAY_BUFFER, (*meshes)[0]->getVBO());
	glBufferData(GL_ARRAY_BUFFER, (*meshes)[0]->vertices.size() * sizeof(Vertex), &(*meshes)[0]->vertices[0], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, cellValuesVBO);
	glBufferData(GL_ARRAY_BUFFER, cellsValues.size() * sizeof(GridCellValues), &cellsValues[0], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0); 

	checkGLError(" - update sph error -");
}

void SPHSystem::uploadParticlesToGPU() {
	/*glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, particleBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, scalarFunctionBuffer);*/
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, p);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, vx);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, vy);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, rho);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, mass);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, fpressx);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, fpressy);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 7, fviscx);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 8, fviscy);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 9, vz);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 10, fpressz);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 11, fviscz);
	
}

void SPHSystem::downloadParticlesFromGPU() {
	
	/*glBindBuffer(GL_SHADER_STORAGE_BUFFER, particleBuffer);
	if (Particle* ptr = (Particle*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY)) {
		std::memcpy(particles.data(), ptr, particles.size() * sizeof(Particle));
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
		for (const auto& particle : particles) {
			std::cout << "Position: ("
				<< particle.position.x << ",\t"
				<< particle.position.y << ",\t"
				<< particle.position.z << ",\t"
				<< ")\n";
		}
	}
	else {
		std::cerr << "Error: glMapBuffer returned NULL\n";
	}*/
	//glBindBuffer(GL_SHADER_STORAGE_BUFFER, p);
	//if (glm::vec4* ptr = (glm::vec4*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY)) {
	//	std::memcpy(&particles[0], ptr, particles.size() * sizeof(glm::vec4));
	//	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	//}
	//else {
	//	std::cerr << "Error: glMapBuffer returned NULL\n";
	//}
	//glBindBuffer(GL_SHADER_STORAGE_BUFFER, rho);
	//int i = 0;
	//if (float* ptr = (float*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY)) {
	//	std::memcpy(&rho_cpu[0], ptr, rho_cpu.size() * sizeof(float));
	//	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	//}
	//else {
	//	std::cerr << "Error: glMapBuffer returned NULL\n";
	//}


	//for (const auto& particle : particles) {
	//	std::cout << "Position : ("
	//		<< i << "    \t"
	//		<< ((i+1 < rho_cpu.size()) ? rho_cpu[i++] : -1) << ",\t"
	//		<< particle.x << ",\t"
	//		<< particle.y << ",\t"
	//		<< particle.z << ",\t"
	//		//<< particle.w << ",\t"
	//		<< ")\n";
	//}
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, scalarFunctionBuffer);
	if (float* ptr2 = (float*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY)) {
		std::memcpy(scalarFunctionFlatted.data(), ptr2, scalarFunctionFlatted.size() * sizeof(float));
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

		int index = 0;
		for (int d = 0; d < attributes->gridSize.x; ++d) {
			for (int r = 0; r < attributes->gridSize.y; ++r) {
				for (int c = 0; c < attributes->gridSize.z; ++c) {
					scalarFunction[d][r][c] = scalarFunctionFlatted[index++];
				}
			}
		}
	}
	else {
		std::cerr << "Error: glMapBuffer returned NULL\n";
	}

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void SPHSystem::bindCellsValuesVBO()
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

void SPHSystem::setupMesh() {
	initGenerators();
	glGenBuffers(1, &cellValuesVBO);
	meshes = std::make_shared<std::vector<Mesh*>>(std::vector<Mesh*>{new Mesh(calculateCellsFirstVertices())});
	(*meshes)[0]->setupMeshWithouIndicesTemp();
	bindCellsValuesVBO();

	glGenBuffers(1, &p);

	glGenBuffers(1, &vx);
	glGenBuffers(1, &vy);
	glGenBuffers(1, &vz);

	glGenBuffers(1, &rho);
	glGenBuffers(1, &mass);

	glGenBuffers(1, &fpressx);
	glGenBuffers(1, &fpressy);
	glGenBuffers(1, &fpressz);

	glGenBuffers(1, &fviscx);
	glGenBuffers(1, &fviscy);
	glGenBuffers(1, &fviscz);
	
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, p);
	glBufferData(GL_SHADER_STORAGE_BUFFER, N3 * sizeof(glm::vec4), &particles[0], GL_DYNAMIC_DRAW);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, vx);
	glBufferData(GL_SHADER_STORAGE_BUFFER, N3 * sizeof(float), &vx_cpu[0], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, vy);
	glBufferData(GL_SHADER_STORAGE_BUFFER, N3 * sizeof(float), &vy_cpu[0], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, vz);
	glBufferData(GL_SHADER_STORAGE_BUFFER, N3 * sizeof(float), &vz_cpu[0], GL_DYNAMIC_DRAW);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, rho);
	glBufferData(GL_SHADER_STORAGE_BUFFER, N3 * sizeof(float), &rho_cpu[0], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, mass);
	glBufferData(GL_SHADER_STORAGE_BUFFER, N3 * sizeof(float), &mass_cpu[0], GL_DYNAMIC_DRAW);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, fpressx);
	glBufferData(GL_SHADER_STORAGE_BUFFER, N3 * sizeof(float), &force_pres_x_cpu[0], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, fpressy);
	glBufferData(GL_SHADER_STORAGE_BUFFER, N3 * sizeof(float), &force_pres_y_cpu[0], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, fpressz);
	glBufferData(GL_SHADER_STORAGE_BUFFER, N3 * sizeof(float), &force_pres_z_cpu[0], GL_DYNAMIC_DRAW);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, fviscx);
	glBufferData(GL_SHADER_STORAGE_BUFFER, N3 * sizeof(float), &force_visc_x_cpu[0], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, fviscy);
	glBufferData(GL_SHADER_STORAGE_BUFFER, N3 * sizeof(float), &force_visc_y_cpu[0], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, fviscz);
	glBufferData(GL_SHADER_STORAGE_BUFFER, N3 * sizeof(float), &force_visc_z_cpu[0], GL_DYNAMIC_DRAW);

	glGenBuffers(1, &scalarFunctionBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, scalarFunctionBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, scalarFunctionFlatted.size() * sizeof(float), &scalarFunctionFlatted[0], GL_DYNAMIC_DRAW);

	//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, scalarFunctionBuffer);
	//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, particleBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, p);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, vx);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, vy);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, rho);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, mass);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, fpressx);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, fpressy);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 7, fviscx);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 8, fviscy);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 9, vz);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 10, fpressz);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 11, fviscz);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void SPHSystem::initGenerators() {
	glm::vec3& noiseGridSize = attributes->gridSize;
	scalarFunction = std::vector<std::vector<std::vector<float>>>(noiseGridSize.x, std::vector<std::vector<float>>(noiseGridSize.y, std::vector<float>(noiseGridSize.z, 1.0f)));
	for (int i = 0; i < attributes->gridSize.x; i++) {
		for (int j = 0; j < attributes->gridSize.y; j++) {
			for (int k = 0; k < attributes->gridSize.z; k++) {
				scalarFunctionFlatted.push_back(1.0f);
			}
		}
	}
	trianglesGenerator = new TrianglesGenerator(noiseGridSize);
	gridGenerator = new GridGenerator(noiseGridSize);
}

bool isWithinSphere(float px, float py, float pz, int x, int y, int z, float radius) {
	return std::sqrt(std::pow(px - x, 2) + std::pow(py - y, 2) + std::pow(pz - z, 2)) <= radius;
}

std::vector<Vertex> SPHSystem::calculateCellsFirstVertices() {
	std::vector<Vertex> vertices;
	cellsValues.clear();
	glm::ivec3 igridSize = attributes->gridSize;
	
	static bool firstCallHolder = true;
	if (firstCallHolder) {
		for (int i = 0; i < particles.size(); i++) {
			//glm::vec3 p = particles[i].position;
			glm::vec3 pos = glm::vec3(particles[i]);
			int startX = std::floor(pos.x - kernelRadius);
			int endX = std::ceil(pos.x + kernelRadius);
			int startY = std::floor(pos.y - kernelRadius);
			int endY = std::ceil(pos.y + kernelRadius);
			int startZ = std::floor(pos.z - kernelRadius);
			int endZ = std::ceil(pos.z + kernelRadius);


			for (int x = startX; x <= endX; ++x) {
				for (int y = startY; y <= endY; ++y) {
					for (int z = startZ; z <= endZ; ++z) {
						if (x >= 0 && x < scalarFunction.size() && y >= 0 && y < scalarFunction[0].size() && z >= 0 && z < scalarFunction[0][0].size()) {
							if (isWithinSphere(pos.x, pos.y, pos.z, x, y, z, kernelRadius)) {
								scalarFunction[x][y][z] = -1.0f;
							}
						}
					}
				}
			}
		}
		firstCallHolder = false;
	}
	

	int ss = 0;
	for (int i = 0; i < (igridSize.x - 1); i++) {
		for (int j = 0; j < (igridSize.y - 1); j++) {
			for (int k = 0; k < (igridSize.z - 1); k++) {
				float	x = i / attributes->gridSize.x - 0.5f,
					y = j / attributes->gridSize.y - 0.5f,
					z = k / attributes->gridSize.z - 0.5f;
				int cubeIndex = 0;
				
				cubeIndex |= ((scalarFunction[i][j][k] < attributes->isoValue ? 1 : 0) << 0);
				cubeIndex |= ((scalarFunction[i + 1][j][k] < attributes->isoValue ? 1 : 0) << 1);
				cubeIndex |= ((scalarFunction[i + 1][j][k + 1] < attributes->isoValue ? 1 : 0) << 2);
				cubeIndex |= ((scalarFunction[i][j][k + 1] < attributes->isoValue ? 1 : 0) << 3);
				cubeIndex |= ((scalarFunction[i][j + 1][k] < attributes->isoValue ? 1 : 0) << 4);
				cubeIndex |= ((scalarFunction[i + 1][j + 1][k] < attributes->isoValue ? 1 : 0) << 5);
				cubeIndex |= ((scalarFunction[i + 1][j + 1][k + 1] < attributes->isoValue ? 1 : 0) << 6);
				cubeIndex |= ((scalarFunction[i][j + 1][k + 1] < attributes->isoValue ? 1 : 0) << 7);
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