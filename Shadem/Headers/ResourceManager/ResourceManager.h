#pragma once

#include "Shader/Shader.h"
#include "Objects/Mesh.h"
#include "map"

class ResourceManager
{
private:
	static std::map<std::string, std::shared_ptr<Shader>> mapOfShaders;
	static std::map<std::string, std::shared_ptr<Mesh>> mapOfMeshes;


public:

	static std::shared_ptr<Shader>& createOrGetShader(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);
	//static std::shared_ptr<Mesh>& createOrGetMesh(const std::string& MeshFilePath);

	static void cleanUp();

};
