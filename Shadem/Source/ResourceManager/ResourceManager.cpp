#define STB_IMAGE_IMPLEMENTATION

#include <initializer_list>
#include "ResourceManager/ResourceManager.h"


std::unordered_map<std::string, std::shared_ptr<Shader>> ResourceManager::mapOfShaders = std::unordered_map<std::string, std::shared_ptr<Shader>>();
std::unordered_map<std::string, std::shared_ptr<Texture>> ResourceManager::mapOfTextures = std::unordered_map<std::string, std::shared_ptr<Texture>>();
std::unordered_map<std::string, std::shared_ptr<std::vector<Mesh*>>> ResourceManager::mapOfMeshes = std::unordered_map<std::string, std::shared_ptr<std::vector<Mesh*>>>();

/* returns pair of key and shader, creates one if doesn't exists under given key */
std::pair<const std::string, std::shared_ptr<Shader>>& ResourceManager::createOrGetShader(std::initializer_list<const std::string> shaderFilePaths)
{
    std::string key;
    for (auto path = shaderFilePaths.begin(); path != shaderFilePaths.end(); path++) {
        key += path->c_str() + std::string("|");
    }
    key.pop_back(); // Remove the last '|'

    auto mapElement = mapOfShaders.find(key);

    if (mapElement == mapOfShaders.end()) {
        mapOfShaders[key] = std::make_shared<Shader>(shaderFilePaths);
        mapElement = mapOfShaders.find(key);
    }
    return *mapElement;
}

/* returns pair of key and model, creates one if doesn't exists under given key */
std::pair<const std::string, std::shared_ptr<std::vector<Mesh*>>>& ResourceManager::createOrGetMesh(const std::string& meshFilePath)
{
	auto mapElement = mapOfMeshes.find(meshFilePath);

	if (mapElement == mapOfMeshes.end()) {
        mapOfMeshes.emplace(meshFilePath, std::make_shared<std::vector<Mesh*>>());
        mapElement = mapOfMeshes.find(meshFilePath);
        loadModel(meshFilePath, (*mapElement).second);
	}
    return *mapElement;
}

/* returns shared pointer to texture slot or creates one if doesn't exists under given key (slot needs to be filled) */
std::shared_ptr<Texture>& ResourceManager::createOrGetTexture(const std::string& textureFilePath)
{
    auto mapElement = mapOfTextures.find(textureFilePath);

    if (mapElement == mapOfTextures.end()) {
        mapOfTextures.emplace(textureFilePath, std::make_shared<Texture>());
        mapElement = mapOfTextures.find(textureFilePath);
    }
    return (*mapElement).second;
}

/* inserts object into Resource Manager under given key if doesn't have one */
void ResourceManager::emplaceMesh(const std::string& meshFilePath, std::shared_ptr<std::vector<Mesh*>>& model)
{
    if (mapOfMeshes.find(meshFilePath) == mapOfMeshes.end())
        mapOfMeshes.emplace(meshFilePath, model);
}

/* returns true if contains object under given key, false otherwise */
bool ResourceManager::containsMesh(const std::string& meshFilePath)
{
    if(mapOfMeshes.find(meshFilePath) == mapOfMeshes.end())
        return false;
    return true;
}

/* returns reference to shared_ptr, can return empty shared_ptr if doesn't contain given key */
std::shared_ptr<std::vector<Mesh*>>& ResourceManager::findMesh(const std::string& meshFilePath)
{
    auto mapElement = mapOfMeshes.find(meshFilePath);
    return (*mapElement).second;
}

/* tries to clear memory from Resource Manager under given key */
void ResourceManager::tryDeleteShader(const std::string& shaderPath)
{
    if(mapOfShaders.find(shaderPath) != mapOfShaders.end() && mapOfShaders[shaderPath].use_count() < 2)
        mapOfShaders.erase(shaderPath);
}

/* tries to clear memory from Resource Manager under given key */
void ResourceManager::tryDeleteMesh(const std::string& meshPath)
{
    if(mapOfMeshes.find(meshPath) != mapOfMeshes.end() && mapOfMeshes[meshPath].use_count() < 2)
        mapOfMeshes.erase(meshPath);
}

/* tries to clear memory from Resource Manager under given key */
void ResourceManager::tryDeleteTexture(const std::string& texturePath)
{
    if (mapOfTextures.find(texturePath) != mapOfTextures.end() && mapOfTextures[texturePath].use_count() < 2)
        mapOfTextures.erase(texturePath);
}

void ResourceManager::loadModel(const std::string& filePath, std::shared_ptr<std::vector<Mesh*>>& meshes)
{
    Assimp::Importer import;
    const aiScene * scene = import.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }

    processNode(scene->mRootNode, scene, filePath, meshes);
}

void ResourceManager::processNode(aiNode* node, const aiScene* scene, const std::string& filePath, std::shared_ptr<std::vector<Mesh*>>& meshes)
{
    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        (*meshes).emplace_back(processMesh(mesh, scene, filePath));
    }
    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene, filePath, meshes);
    }
}

Mesh* ResourceManager::processMesh(aiMesh* mesh, const aiScene* scene, const std::string& filePath)
{
    std::vector<Vertex> vertices(mesh->mNumVertices, Vertex());
    std::vector<unsigned int> indices(mesh->mNumFaces * 3);
    std::vector<std::shared_ptr<Texture>> textures;

    //process vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
    {
        Vertex vertex;
        vertex.position.x = mesh->mVertices[i].x;
        vertex.position.y = mesh->mVertices[i].y;
        vertex.position.z = mesh->mVertices[i].z;

        if (mesh->HasNormals()) {
            vertex.normal.x = mesh->mNormals[i].x;
            vertex.normal.y = mesh->mNormals[i].y;
            vertex.normal.z = mesh->mNormals[i].z;
        }

        if (mesh->HasTextureCoords(0))
        {
            vertex.texCoords.x = mesh->mTextureCoords[0][i].x;
            vertex.texCoords.y = mesh->mTextureCoords[0][i].y;

            if (mesh->HasTangentsAndBitangents()) {
                vertex.tangent.x = mesh->mTangents[i].x;
                vertex.tangent.y = mesh->mTangents[i].y;
                vertex.tangent.z = mesh->mTangents[i].z;

                vertex.bitangent.x = mesh->mBitangents[i].x;
                vertex.bitangent.y = mesh->mBitangents[i].x;
                vertex.bitangent.z = mesh->mBitangents[i].x;
            }
        }
        else 
        {
            vertex.texCoords = glm::vec2(0.0f, 0.0f);
        }

        vertices[i] = std::move(vertex);
    }

    // process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
    {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for (unsigned int j = 0; j < face.mNumIndices; ++j)
            indices[i * face.mNumIndices + j] = face.mIndices[j];
    }

    // process material
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        
        std::vector<std::shared_ptr<Texture>> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", filePath);
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        std::vector<std::shared_ptr<Texture>> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular", filePath);
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    return new Mesh(vertices, indices, textures);
}

std::vector<std::shared_ptr<Texture>> ResourceManager::loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName, const std::string& filePath)
{
    std::vector<std::shared_ptr<Texture>> textures;
    std::shared_ptr<Texture> texture;

    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);

        std::string textureFilePath = filePath.substr(0, filePath.find_last_of("/") + 1) + str.C_Str();

        texture = createOrGetTexture(textureFilePath);
        if (texture->path.empty())
        {
            texture->id = TextureFromFile(textureFilePath);
            texture->type = typeName;
            texture->path = textureFilePath;
        }

        textures.push_back(texture);
    }
    return textures;
}

unsigned int ResourceManager::TextureFromFile(const std::string& filePath)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << filePath << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

