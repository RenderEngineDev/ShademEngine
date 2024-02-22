#pragma once
#include <Shader/Shader.h>

//#include <glad/glad.h>
#include "CubeMarching/Types.h"
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
	Vertex(glm::vec3 position, glm::vec2 texCoords) : position(position), texCoords(texCoords), normal(glm::vec3(0.0f, 0.0f, -1.0f)) {};
	Vertex(Point p, glm::vec3 normal = glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2 texCoords = glm::vec2(0.0f), glm::vec3 tangent = glm::vec3(0.0f), glm::vec3 bitangent = glm::vec3(0.0f)) : position(glm::vec3(p.x, p.y, p.z)) {};
	Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 texCoords, glm::vec3 tangent, glm::vec3 bitangent) : position(position), normal(normal), tangent(tangent), bitangent(bitangent) {};
	Vertex(glm::vec3 position, glm::vec3 normal) : position(position), normal(normal) {};
	Vertex(glm::vec3 position) : position(position) {};
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
	void DrawEdges(std::shared_ptr<Shader>& shader);
	void DrawWithoutIndices(std::shared_ptr<Shader>& shader);
	void DrawPatches();
	void DrawRectangleForComputeShader();
	void DrawGeoPointsCM();

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