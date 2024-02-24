#include "ResourceManager/ResourceManager.h"

std::map<std::string, std::shared_ptr<Shader>> ResourceManager::mapOfShaders = std::map<std::string, std::shared_ptr<Shader>>();
std::map<std::string, std::shared_ptr<Mesh>> ResourceManager::mapOfMeshes = std::map<std::string, std::shared_ptr<Mesh>>();


std::shared_ptr<Shader>& ResourceManager::createOrGetShader(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath)
{
	auto shader = mapOfShaders.find(vertexShaderFilePath + "|" + fragmentShaderFilePath);

	if (shader == mapOfShaders.end()) {
		mapOfShaders.emplace(vertexShaderFilePath + "|" + fragmentShaderFilePath, std::make_shared<Shader>(vertexShaderFilePath.c_str(), fragmentShaderFilePath.c_str()));
		return mapOfShaders[vertexShaderFilePath + "|" + fragmentShaderFilePath];
	}

	return (*shader).second;
}


//std::shared_ptr<Mesh>& ResourceManager::createOrGetMesh(const std::string& MeshFilePath)
//{
//	std::shared_ptr<Mesh> mesh = MapOfMeshes.find(mesh);
//
//	if (mesh == MapOfMeshes.end()) {
//		MapOfMeshes.emplace(mesh, std::make_shared<Mesh>(MeshFilePath.c_str()));
//		return MapOfMeshes[mesh];
//	}
//
//	return shader
//}

void ResourceManager::cleanUp()
{
	mapOfShaders.clear();
}
