#define STB_IMAGE_IMPLEMENTATION

#include "ResourceManager/ResourceManager.h"

std::unordered_map<std::string, std::shared_ptr<Shader>> ResourceManager::mapOfShaders = std::unordered_map<std::string, std::shared_ptr<Shader>>();
std::unordered_map<std::string, std::shared_ptr<std::vector<Mesh>>> ResourceManager::mapOfMeshes = std::unordered_map<std::string, std::shared_ptr<std::vector<Mesh>>>();


std::pair<const std::string&, std::shared_ptr<Shader>&> ResourceManager::createOrGetShader(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath)
{
	auto mapElement = mapOfShaders.find(vertexShaderFilePath + "|" + fragmentShaderFilePath);

	if (mapElement == mapOfShaders.end()) {
		mapOfShaders.emplace(vertexShaderFilePath + "|" + fragmentShaderFilePath, std::make_shared<Shader>(vertexShaderFilePath.c_str(), fragmentShaderFilePath.c_str()));
		mapElement = mapOfShaders.find(vertexShaderFilePath + "|" + fragmentShaderFilePath);

        std::cout << "Shader map: " << mapOfShaders.size() << " (ins)\n";
		return std::pair<const std::string&, std::shared_ptr<Shader>&>((*mapElement).first, (*mapElement).second);
	}

    std::cout << "Shader map: " << mapOfShaders.size() << " (ins)\n";
	return std::pair<const std::string&, std::shared_ptr<Shader>&>((*mapElement).first, (*mapElement).second);
}


std::pair<const std::string&, std::shared_ptr<std::vector<Mesh>>&> ResourceManager::createOrGetMesh(const std::string& meshFilePath)
{
	auto mapElement = mapOfMeshes.find(meshFilePath);

	if (mapElement == mapOfMeshes.end()) {
		mapOfMeshes.emplace(meshFilePath, std::make_shared<std::vector<Mesh>>());
        mapElement = mapOfMeshes.find(meshFilePath);

        loadModel(meshFilePath, (*mapElement).second);

        std::cout << "Mesh map: " << mapOfMeshes.size() << " (ins)\n";
        return std::pair<const std::string&, std::shared_ptr<std::vector<Mesh>>&>((*mapElement).first, (*mapElement).second);
	}

    std::cout << "Mesh map: " << mapOfMeshes.size() << " (ins)\n";
    for (auto x : mapOfMeshes)
        std::cout << x.first << "\n";

    return std::pair<const std::string&, std::shared_ptr<std::vector<Mesh>>&>((*mapElement).first, (*mapElement).second);
}

void ResourceManager::tryDeleteShader(std::string& shaderPath)
{
    mapOfShaders.erase(shaderPath);

    std::cout << "Shader map: " << mapOfShaders.size() << " (del)\n";
}

void ResourceManager::tryDeleteMesh(std::string& meshPath)
{
    mapOfMeshes.erase(meshPath);

    std::cout << "Mesh map: " << mapOfMeshes.size() << " (del)\n";
}

void ResourceManager::loadModel(const std::string& filePath, std::shared_ptr<std::vector<Mesh>>& meshes)
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

void ResourceManager::processNode(aiNode* node, const aiScene* scene, const std::string& filePath, std::shared_ptr<std::vector<Mesh>>& meshes)
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

Mesh ResourceManager::processMesh(aiMesh* mesh, const aiScene* scene, const std::string& filePath)
{
    std::vector<Vertex> vertices(mesh->mNumVertices, Vertex());
    std::vector<unsigned int> indices(mesh->mNumFaces * 3);
    std::vector<Texture> textures;

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

        vertices[i] = vertex;
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
        std::vector<Texture> diffuseMaps = loadMaterialTextures(material,
            aiTextureType_DIFFUSE, "texture_diffuse", filePath);
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<Texture> specularMaps = loadMaterialTextures(material,
            aiTextureType_SPECULAR, "texture_specular", filePath);
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    return Mesh(vertices, indices, textures);
}

std::vector<Texture> ResourceManager::loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName, const std::string& filePath)
{
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        Texture texture;
        texture.id = TextureFromFile(str.C_Str(), filePath.substr(0, filePath.find_first_of('/')));
        texture.type = typeName;
        texture.path = str.C_Str();
        textures.push_back(texture);
    }
    return textures;
}

unsigned int ResourceManager::TextureFromFile(const char* path, const std::string& directory)
{
    std::string filename = std::string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
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
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

void ResourceManager::cleanUp()
{
	mapOfShaders.clear();
}
