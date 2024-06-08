#include "RayMarching/SPH.h"

void SPH::init()
{
	auto shaderPair = ResourceManager::createOrGetShader({ Shaders::getRmVertexShaderPath(), "../Shadem/Shaders/RMShaders/SPH/SPHFshader.shader"});
	this->shader = shaderPair.second;
	this->setShaderResourceKey(shaderPair.first);

	particleShader = new Shader({ "../Shadem/Shaders/RMShaders/SPH/particles.shader" });
	forcesShader = new Shader({ "../Shadem/Shaders/RMShaders/SPH/forces.shader" });
	densityShader = new Shader({"../Shadem/Shaders/RMShaders/SPH/density.shader"});

	setSPHUnifroms(particleShader);
	setSPHUnifroms(forcesShader);
	setSPHUnifroms(densityShader);

	particles.resize(N);
	particlesCopy.resize(N);
	vx_cpu.resize(N);
	vy_cpu.resize(N);
	vz_cpu.resize(N);
	mass_cpu.resize(N);


	dis = std::uniform_real_distribution<float>(0.25f, 0.5f);

	int k = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			for (int m = 0; m < n; m++)
			{
				particles[k].x = 0.25 + 0.25 * i / float(n); //dis(gen32); //0.25 + 0.25 * i / float(n);//ofRandom(0,W)/float(W);
				particles[k].y = 0.25 + 0.25 * j / float(n); //dis(gen32); //0.25 + 0.25 * j / float(n);//ofRandom(0,H)/float(H);
				particles[k].z = 0.25 + 0.25 * m / float(n); //dis(gen32); //0.25 + 0.25 * m / float(n);//1-2*1*0.5*w/float(N3);
				particles[k].w = 1;
				vx_cpu[k] = vy_cpu[k] = vz_cpu[k] = 0;
				mass_cpu[k] = mass0;
				k++;
			}
		}
	}
	
	genBuffers();
	allocatebuffers();
	bindBuffers();

	shader->use();
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, p);
	shader->setInt("N",N);
	shader->setFloat("ballScale", ballScale);
}

void SPH::genBuffers()
{
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
}

void SPH::allocatebuffers()
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, p);
	glBufferData(GL_SHADER_STORAGE_BUFFER, N * sizeof(glm::vec4), &particles[0], GL_DYNAMIC_DRAW);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, vx);
	glBufferData(GL_SHADER_STORAGE_BUFFER, N * sizeof(float), &vx_cpu[0], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, vy);
	glBufferData(GL_SHADER_STORAGE_BUFFER, N * sizeof(float), &vy_cpu[0], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, vz);
	glBufferData(GL_SHADER_STORAGE_BUFFER, N * sizeof(float), &vz_cpu[0], GL_DYNAMIC_DRAW);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, rho);
	glBufferData(GL_SHADER_STORAGE_BUFFER, N * sizeof(float), &rho_cpu[0], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, mass);
	glBufferData(GL_SHADER_STORAGE_BUFFER, N * sizeof(float), &mass_cpu[0], GL_DYNAMIC_DRAW);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, fpressx);
	glBufferData(GL_SHADER_STORAGE_BUFFER, N * sizeof(float), &force_pres_x_cpu[0], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, fpressy);
	glBufferData(GL_SHADER_STORAGE_BUFFER, N * sizeof(float), &force_pres_y_cpu[0], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, fpressz);
	glBufferData(GL_SHADER_STORAGE_BUFFER, N * sizeof(float), &force_pres_z_cpu[0], GL_DYNAMIC_DRAW);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, fviscx);
	glBufferData(GL_SHADER_STORAGE_BUFFER, N * sizeof(float), &force_visc_x_cpu[0], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, fviscy);
	glBufferData(GL_SHADER_STORAGE_BUFFER, N * sizeof(float), &force_visc_y_cpu[0], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, fviscz);
	glBufferData(GL_SHADER_STORAGE_BUFFER, N * sizeof(float), &force_visc_z_cpu[0], GL_DYNAMIC_DRAW);
}

void SPH::bindBuffers()
{
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

void SPH::setSPHUnifroms(Shader* shader)
{
	shader->use();
	float h = 0.03;
	float de = 0.02;
	float rho0 = 5.0;
	//float mass0 = 0.003;
	float mu = 10.0;

	float gz = 0;//0.3;
	float gy = GZ;//0.3;
	float gx = 0;
	float dt = 0.0025;
	float k = 8.0;                 // J/(mol*K) - gas constant
	float M_PI = 3.14159265359;

	shader->setInt("N", N);
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
}

SPH::SPH()
{
	init();
}

SPH::SPH(ObjectAttributes::Common* _attributes)
{
	init();
	attributes = _attributes;
}

void SPH::draw(Camera::Camera& camera)
{
	shader->use();
	setUniforms(camera);

	shader->setVec3("SpherePos", attributes->position);
	shader->setVec3("Scale", attributes->scale);

	for (auto& mesh : *meshes)
		mesh->Draw(shader);
}

void SPH::update(Camera::Camera& camera)
{
	densityShader->use();
	densityShader->dispatch((N+63)/64, 1, 1);
	forcesShader->use();
	forcesShader->dispatch((N + 63) / 64, 1, 1);
	particleShader->use();
	particleShader->dispatch((N + 63) / 64, 1, 1);

	t += 0.04;

	//particleShader->setFloat("gz", -2 * GZ * sin(t * 0.1));
	//particleShader->setFloat("gx", -2 * GZ * sin(t * 0.1 + 0.5));


	//glBindBuffer(GL_SHADER_STORAGE_BUFFER, p);
	//glm::vec4* read_data = (glm::vec4*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
	//for (int i = 0; i < particles.size(); ++i) {
	//	particlesCopy[i] = read_data[i];
	//	std::cout << particlesCopy[i].x << " " << particlesCopy[i].y << " " << particlesCopy[i].z << "\n";
	//}
	//glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

}
