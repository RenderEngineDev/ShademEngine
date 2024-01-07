#pragma once

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

	Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 texCoords, glm::vec3 tangent, glm::vec3 bitangent) : position(position), normal(normal), tangent(tangent), bitangent(bitangent) {};
};

struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};


class Mesh
{
public:
	// mesh data
	std::vector<Vertex>			vertices;
	std::vector<unsigned int>	indices;
	std::vector<Texture>		textures;
		
	Mesh() {};
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	Mesh(std::vector<Vertex> vertices, std::vector<Texture> textures);

	void Draw();
	void DrawWithoutIndices();

	unsigned int getVAO() const;
	unsigned int getVBO() const;
	unsigned int getEBO() const;

private:
	// render data
	unsigned int VAO, VBO, EBO;

	void setupMesh();

	void setupMeshWithouIndices();

};