#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <stb_image.h>

class Shader
{
	public: 
		// the program ID
		unsigned int ID;
		unsigned int texture;

		// constructor reads and builds the shader
		Shader() {};
		Shader(const char* vertexPath, const char* fragmentPath);
		// use/activate the shader
		void use();
		// utility uniform functions
		void setBool(const std::string& name, bool value) const;
		void setInt(const std::string& name, int value) const;
		void setFloat(const std::string& name, float value) const;
		void setVec2(const std::string& name, const glm::vec2 &vec) const;
		void setVec3(const std::string& name, const glm::vec3& vec) const;
		void setVec4(const std::string& name, const glm::vec4& vec) const;
		void setMat2(const std::string& name, glm::mat4 &mat) const;
		void setMat3(const std::string& name, glm::mat4 &mat) const;
		void setMat4(const std::string& name, glm::mat4& value) const;
		void setTexture2D(const std::string& name);
private:
		void checkCompileErrors(GLuint shader, std::string type);
};