#pragma once
#include <Shader/Shader.h>

//#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <vector>


struct Vertex {
	// position
	glm::vec3 position;
	// normal
	glm::vec3 normal;
	// texCoords
	glm::vec2 texCoords;
	// tangent
	glm::vec3 tangent;
	// bitangent
	glm::vec3 bitangent;

	Vertex() {};
	Vertex(glm::vec3 position, glm::vec3 normal = glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2 texCoords = glm::vec2(0.0f), glm::vec3 tangent = glm::vec3(0.0f), glm::vec3 bitangent = glm::vec3(0.0f)) : position(position), normal(normal), tangent(tangent), bitangent(bitangent) {};
};

struct Texture 
{
public:

	unsigned int id;
	std::string type;
	std::string path;

	~Texture() 
	{
		glDeleteTextures(1, &id);
	};
};


class Mesh
{
public:
	// mesh data
	std::vector<Vertex>					  vertices;
	std::vector<unsigned int>			  indices;
	std::vector<std::shared_ptr<Texture>> textures;
		
	Mesh();
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<std::shared_ptr<Texture>> textures);
	Mesh(std::vector<Vertex> vertices, std::vector<std::shared_ptr<Texture>> textures);

	void Draw(std::shared_ptr<Shader>& shader);
	void DrawWithoutIndices(std::shared_ptr<Shader>& shader);

	unsigned int getVAO() const;
	unsigned int getVBO() const;
	unsigned int getEBO() const;

	void setupMesh();

	void setupMeshWithouIndices();

	~Mesh();

private:
	// render data
	unsigned int VAO, VBO, EBO;

	void GenBuffers();

};