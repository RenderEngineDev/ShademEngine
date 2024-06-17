#include "RayMarching/SPH.h"

void SPH::init()
{
	auto shaderPair = ResourceManager::createOrGetShader({ Shaders::getRmVertexShaderPath(), "../Shadem/Shaders/RMShaders/SPH/SPHFshader.shader" });//"../Shadem/Shaders/RMShaders/SPH/SPHFshader.shader"
	this->shader = shaderPair.second;
	this->setShaderResourceKey(shaderPair.first);

	particleShader = new Shader({ "../Shadem/Shaders/RMShaders/SPH/particles.shader" });
	forcesShader = new Shader({ "../Shadem/Shaders/RMShaders/SPH/forces.shader" });
	densityShader = new Shader({"../Shadem/Shaders/RMShaders/SPH/density.shader"});

	setSPHUnifroms(particleShader);
	setSPHUnifroms(forcesShader);
	setSPHUnifroms(densityShader);

	particles.resize(N);
	projectedParticles.resize(N);
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
				particles[k].x = dis(gen32); //0.25 + 0.25 * i / float(n);//ofRandom(0,W)/float(W);
				particles[k].y = dis(gen32); //0.25 + 0.25 * j / float(n);//ofRandom(0,H)/float(H);
				particles[k].z = dis(gen32); //0.25 + 0.25 * m / float(n);//1-2*1*0.5*w/float(N3);
				//particles[k] = glm::vec4(0.5, 0, 0.5, 1);
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
	//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 12, projectedP);
	shader->setInt("N",N);
	shader->setFloat("ballScale", ballScale);

	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, RMresolution.x, RMresolution.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, RMresolution.x, RMresolution.y, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &colorTexture);
	glBindTexture(GL_TEXTURE_2D, colorTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, RMresolution.x, RMresolution.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture, 0);

	//glGenRenderbuffers(1, &RBO);
	//glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_RGB, RMresolution.x, RMresolution.y);
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, RBO);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	std::shared_ptr<Texture> t = std::make_shared<Texture>();
	t->id = colorTexture;
	(*meshes)[0]->textures.emplace_back(t);


	t = std::make_shared<Texture>();
	t->id = depthTexture;
	(*meshes)[0]->textures.emplace_back(t);

	//glBindVertexArray((*meshes)[0]->getVAO());
	//glBindTexture(GL_TEXTURE_2D, texture);
	//glBindVertexArray(0);
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

	glGenBuffers(1, &projectedP);
}

void SPH::allocatebuffers()
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, p);
	glBufferData(GL_SHADER_STORAGE_BUFFER, N * sizeof(glm::vec4), &particles[0], GL_DYNAMIC_DRAW);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, projectedP);
	glBufferData(GL_SHADER_STORAGE_BUFFER, N * sizeof(glm::vec4), &projectedParticles[0], GL_DYNAMIC_DRAW);

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
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 12, projectedP);
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
	//glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	//glClearColor(0.2f, 0.3f, 0.7f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader->use();
	setUniforms(camera);
	//shader->setBool("sampleDepth", false);
	shader->setVec3("SPHPos", attributes->position);
	shader->setVec3("Scale", attributes->scale);
	//shader->setVec2("WindowSize", camera.window->getWindowSize());

	//shader->setInt("colorSampler", 0);
	//shader->setInt("depthSampler", 1);

	//for (auto& mesh : *meshes)
	//	mesh->Draw(shader);


	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glClearColor(0.2f, 0.3f, 0.7f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glBindTexture(GL_TEXTURE_2D, Depthtexture);
	//glBindFramebuffer(GL_READ_FRAMEBUFFER, RBO);
	//glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	//glBlitFramebuffer(0, 0, RMresolution.x, RMresolution.y, 0, 0, 1920, 1080 , GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT, GL_NEAREST);

	shader->setBool("sampleDepth", false);
	shader->setVec2("WindowSize", camera.window->getWindowSize());

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
	glm::mat4& model = glm::mat4(1.0f);
	evaluateBasicModelTransformations(model);
	particleShader->setMat4("projection", camera.getProjection() * camera.getView() * model);
	particleShader->setVec2("SPHSphereRadius", glm::vec4(attributes->scale, ballScale));
	particleShader->dispatch((N + 63) / 64, 1, 1);

	t += 0.04;

	particleShader->setFloat("gz", -2 * GZ * sin(t * 0.1));
	//particleShader->setFloat("gx", -2 * GZ * sin(t * 0.1 + 0.5));

	//glBindBuffer(GL_SHADER_STORAGE_BUFFER, projectedP);
	//glm::vec4* read_data = (glm::vec4*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
	//for (int i = 0; i < projectedParticles.size(); ++i) {
	//	projectedParticles[i] = read_data[i];
	//	//particles[i].y = 0;
	//	std::cout << particles[i].x << " " << particles[i].y << " " << particles[i].z << " | ";
	//	//projectedParticles[i] = camera.getProjection() * camera.getView() * model * glm::vec4(particles[i].x - 0.5f, particles[i].y - 0.5f, particles[i].z - 0.5f, particles[i].w);
	//	//projectedParticles[i] /= projectedParticles[i].w;
	//	std::cout << projectedParticles[i].x << " " << projectedParticles[i].y << " " << projectedParticles[i].z << " " << projectedParticles[i].w << "\n";
	//}
	//glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

	//glm::vec2 uv = (glm::vec2(1920,1080/2) - 0.5f * glm::vec2(1920,1080));

	//glm::vec3 rayDirectionBeforeRotation = glm::vec3(uv.x, uv.y, 0);

	//std::cout << rayDirectionBeforeRotation.x << " " << rayDirectionBeforeRotation.y << " " << rayDirectionBeforeRotation.z << " | ";
	//rayDirectionBeforeRotation = glm::normalize(rayDirectionBeforeRotation);
	//std::cout << rayDirectionBeforeRotation.x << " " << rayDirectionBeforeRotation.y << " " << rayDirectionBeforeRotation.z << "\n";

}
