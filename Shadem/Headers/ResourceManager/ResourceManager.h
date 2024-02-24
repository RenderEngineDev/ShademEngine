#pragma once

#include "Shader/Shader.h"
#include "Objects/Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string_view>

#include <vector>
#include <map>
#include <memory>

#define ASSIMP_LOAD_FLAGS (aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices)

class ResourceManager
{
private:

	static std::unordered_map<std::string, std::shared_ptr<Shader>> mapOfShaders;
	static std::unordered_map<std::string, std::shared_ptr<Texture>> mapOfTextures;
	static std::unordered_map<std::string, std::shared_ptr<std::vector<Mesh*>>> mapOfMeshes;

	static void loadModel(const std::string& filePath, std::shared_ptr<std::vector<Mesh*>>& meshes);
	static void processNode(aiNode* node, const aiScene* scene, const std::string& filePath, std::shared_ptr<std::vector<Mesh*>>& meshes);
	static Mesh* processMesh(aiMesh* mesh, const aiScene* scene, const std::string& filePath);
	static std::vector<std::shared_ptr<Texture>> loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName, const std::string& filePath);
	static unsigned int TextureFromFile(const std::string& filePath);

public:

	static std::pair<const std::string, std::shared_ptr<Shader>>& createOrGetShader(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);
	static std::pair<const std::string, std::shared_ptr<std::vector<Mesh*>>>& createOrGetMesh(const std::string& meshFilePath);
	static std::shared_ptr<Texture>& createOrGetTexture(const std::string& textureFilePath);

	static void emplaceMesh(const std::string& meshFilePath, std::shared_ptr<std::vector<Mesh*>>& model);
	static bool containsMesh(const std::string& meshFilePath);
	static std::shared_ptr<std::vector<Mesh*>>& findMesh(const std::string& meshFilePath);

	static void tryDeleteShader(const std::string& shaderPath);
	static void tryDeleteMesh(const std::string& meshPath);
	static void tryDeleteTexture(const std::string& texturePath);

};
