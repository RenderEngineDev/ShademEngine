#pragma once

#include "Shader/Shader.h"
#include "Objects/Mesh.h"
#include "map"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string_view>

#include <vector>

#define ASSIMP_LOAD_FLAGS (aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices)

class ResourceManager
{
public:

	static std::unordered_map<std::string, std::shared_ptr<Shader>> mapOfShaders;
	static std::unordered_map<std::string, std::shared_ptr<std::vector<Mesh>>> mapOfMeshes;
private:
	static void loadModel(const std::string& filePath, std::shared_ptr<std::vector<Mesh>>& meshes);
	static void processNode(aiNode* node, const aiScene* scene, const std::string& filePath, std::shared_ptr<std::vector<Mesh>>& meshes);
	static Mesh processMesh(aiMesh* mesh, const aiScene* scene, const std::string& filePath);
	static std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName, const std::string& filePath);
	static unsigned int TextureFromFile(const char* path, const std::string& filePath);

public:

	static std::pair<const std::string&, std::shared_ptr<Shader>&> createOrGetShader(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);
	static std::pair<const std::string&, std::shared_ptr<std::vector<Mesh>>&> createOrGetMesh(const std::string& MeshFilePath);

	static void tryDeleteShader(std::string& shaderPath);
	static void tryDeleteMesh(std::string& meshPath);


	static void cleanUp();

};
