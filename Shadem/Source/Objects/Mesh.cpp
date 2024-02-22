#pragma once

#include "Objects/Mesh.h"
#include "ResourceManager/ResourceManager.h"

Mesh::Mesh()
{
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<std::shared_ptr<Texture>> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	GenBuffers();

	setupMesh();
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<std::shared_ptr<Texture>> textures)
{
	this->vertices = vertices;
	this->textures = textures;

	GenBuffers();

	setupMeshWithouIndices();
}

void Mesh::setupMesh()
{
	if (!vertices.empty()) {
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
		// vertex positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		// vertex normals
		// The offset is now defined using the OFFSETOF macro that, in this case, sets the byte offset of the normal vector equal to the byte offset of the normal attribute in the struct which is 3 floats and thus 12 bytes
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));	
		// vertex texture coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
		glBindVertexArray(0);
	}
	else {
		// Do zabezpieczenia wyj¹tkami
		std::cout << "WARNING: Binding BufferData of mesh failed: vertices are empty" << std::endl;
	}
}

void Mesh::setupMeshWithouIndices()
{
	if (!vertices.empty()) {
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_DYNAMIC_DRAW);
		// vertex positions	
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		// vertex normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));	
		// vertex texture coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
		glBindVertexArray(0);
	} else {
		std::cout << "WARNING: Binding BufferData of mesh failed: vertices are empty" << std::endl;
	}
}

Mesh::~Mesh()
{
	std::string path;
	for (auto& texture : textures) {
		path = (*texture).path;
		texture.reset();
		ResourceManager::tryDeleteTexture(path);
	}

	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
}

void Mesh::GenBuffers()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
}

//void Mesh::draw()
void Mesh::Draw(std::shared_ptr<Shader>& shader)
{
	//shader.use();
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	//unsigned int normalNr = 1;
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // activate texture unit first
		// retrieve texture number (the N in diffuse_textureN)
		std::string number;
		std::string name = textures[i]->type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++);
		//shader->setFloat(("material." + name + number).c_str(), i);
		glBindTexture(GL_TEXTURE_2D, textures[i]->id);
	}
	//glActiveTexture(GL_TEXTURE0);
	 //draw mesh
	shader->setInt("TextureQuantity", textures.size());

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

//void Mesh::DrawWithoutIndices(Shader& shader)
void Mesh::DrawWithoutIndices(std::shared_ptr<Shader>& shader)
{
	//shader.use();
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	//unsigned int normalNr = 1;
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // activate texture unit first
		// retrieve texture number (the N in diffuse_textureN)
		std::string number;
		std::string name = textures[i]->type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++);
		//shader->setFloat(("material." + name + number).c_str(), i);
		glBindTexture(GL_TEXTURE_2D, textures[i]->id);
	}
	//glActiveTexture(GL_TEXTURE0);
	// draw mesh

	shader->setInt("TextureQuantity", textures.size());

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, (int)vertices.size());
	glBindVertexArray(0);
}

//void Mesh::DrawWithoutIndices(Shader& shader)
void Mesh::DrawRectangleForComputeShader()
{
	//// draw mesh
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
	//glDrawArrays(GL_TRIANGLES, 0, (int)vertices.size());
}

//void Mesh::DrawWithoutIndices(Shader& shader)
void Mesh::DrawPatches()
{
	glBindVertexArray(VAO);
	glDrawArrays(GL_PATCHES, 0, (int)vertices.size());
	glBindVertexArray(0);
}

void Mesh::DrawGeoPointsCM()
{
	glBindVertexArray(VAO);
	glDrawArrays(GL_POINTS, 0, (int)vertices.size());
	glBindVertexArray(0);
}

unsigned int Mesh::getVAO() const {
	return VAO;
}

unsigned int Mesh::getVBO() const {
	return VBO;
}

unsigned int Mesh::getEBO() const {
	return EBO;
}

