#pragma once

#include <Objects/RMObject.h>
#include <random>

class SPH : public RMObject {

private:
	void init();
	void genBuffers();
	void allocatebuffers();
	void bindBuffers();

	Shader* particleShader;
	Shader* forcesShader;
	Shader* densityShader;

	std::mt19937 gen32;
	std::uniform_real_distribution<float> dis;

	float t = 0.01f;
	float ballScale = 0.01f;

	int n = 5;
	int N = n * n * n;
	unsigned int p, vx, vy, vz, rho, mass, fpressx, fpressy, fpressz, fviscx, fviscy, fviscz, projectedP;
	unsigned int FBO, depthTexture, colorTexture, RBO, MSAA;

	const float GZ = -0.25;
	int saveonoff = 0;
	float mass0 = 0.00003;

	glm::vec2 RMresolution = glm::vec2(1920, 1080);

	std::vector<float> vx_cpu, vy_cpu, vz_cpu, rho_cpu, mass_cpu;
	std::vector<float> force_pres_x_cpu, force_pres_y_cpu, force_pres_z_cpu, force_visc_x_cpu, force_visc_y_cpu, force_visc_z_cpu;
	std::vector<glm::vec4> particles, projectedParticles;
	std::vector<glm::vec4> particlesCopy;

	void setSPHUnifroms(Shader* shader);

public:

	SPH();
	SPH(ObjectAttributes::Common* attributes);

	virtual void draw(Camera::Camera& camera) override;
	virtual void update(Camera::Camera& camera) override;

};