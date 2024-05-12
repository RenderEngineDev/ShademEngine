#pragma once

#include <filesystem>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <stb_image.h>
#include <fstream>
#include <sstream>
#include <string>

#define _REGEX_MAX_STACK_COUNT 0 //<-- 0 = unlimited (fucked up on debug mode) // 200000
#include <regex>

#include "Shader/Shader.h"


Shader::Shader(std::initializer_list<const std::string> shaderPaths) {
	auto shaderPath = shaderPaths.begin();
	switch (shaderPaths.size()) {
	case 1: {// c
		compileShaders(shaderPath->c_str());
		break;
	}
	case 2: {// v, f
		compileShaders(shaderPath->c_str(), (shaderPath + 1)->c_str());
		break;
	}
	case 3: { // v, f, g
		compileShaders(shaderPath->c_str(), (shaderPath + 1)->c_str(), (shaderPath + 2)->c_str());
		break;
	} 
	case 4: { // v, f, tc, tv
		compileShaders(shaderPath->c_str(), (shaderPath + 1)->c_str(), (shaderPath + 2)->c_str(), (shaderPath + 3)->c_str());
		break;
	}
	default:
		break;
	}
}

void Shader::compileShaders(const char* vertexPath, const char* fragmentPath)
{
	// 1. retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure& e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what() << std::endl;
	}
	// 2. compile shaders
	unsigned int vertex, fragment;
	// vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	compileShader(vertex, vertexCode);
	checkCompileErrors(vertex, "VERTEX");
	// fragment Shader
	fragmentCode = processDirectives(fragmentCode);
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	compileShader(fragment, fragmentCode);
	checkCompileErrors(fragment, "FRAGMENT");

	// shader Program
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	checkCompileErrors(ID, "PROGRAM");
	// delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(vertex);
	glDeleteShader(fragment);

}

void Shader::compileShaders(const char* vertexPath, const char* fragmentPath, const char* geometryPath)
{
	// 1. retrieve the vertex/fragment/geometry source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	std::ifstream gShaderFile;
	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		gShaderFile.open(geometryPath);
		std::stringstream vShaderStream, fShaderStream, gShaderStream;
		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		gShaderStream << gShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		gShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
		geometryCode = gShaderStream.str();
	}
	catch (std::ifstream::failure& e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what() << std::endl;
	}
	// 2. compile shaders
	unsigned int vertex, fragment, geometry;
	// vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	compileShader(vertex, vertexCode);
	checkCompileErrors(vertex, "VERTEX");
	// fragment Shader
	fragmentCode = processDirectives(fragmentCode);
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	compileShader(fragment, fragmentCode);
	checkCompileErrors(fragment, "FRAGMENT");
	// geometry Shader
	std::string geoShaderSource = geometryCode;
	// TODO: !!!! temporary implementation - in future more generic paths loading while object init !!!!
	geoShaderSource = processDirectives(geoShaderSource);
	geometry = glCreateShader(GL_GEOMETRY_SHADER);
	compileShader(geometry, geoShaderSource);
	//glShaderSource(geometry, 1, &gShaderCode, NULL);
	//glCompileShader(geometry);
	checkCompileErrors(geometry, "GEOMETRY");
	// shader Program
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glAttachShader(ID, geometry);
	glLinkProgram(ID);
	checkCompileErrors(ID, "PROGRAM");
	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	glDeleteShader(geometry);
}


void Shader::compileShaders(const char* vertexPath, const char* fragmentPath, const char* tessControlPath, const char* tessEvalPath)
{
	// 1. retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::string tessControlCode;
	std::string tessEvalCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	std::ifstream tcShaderFile;
	std::ifstream teShaderFile;
	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	tcShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	teShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		tcShaderFile.open(tessControlPath);
		teShaderFile.open(tessEvalPath);
		std::stringstream vShaderStream, fShaderStream, tcShaderStream, teShaderStream;
		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		tcShaderStream << tcShaderFile.rdbuf();
		teShaderStream << teShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		tcShaderFile.close();
		teShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
		tessControlCode = tcShaderStream.str();
		tessEvalCode = teShaderStream.str();
	}
	catch (std::ifstream::failure& e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what() << std::endl;
	}
	// 2. compile shaders
	unsigned int vertex, fragment, tessControl, tessEval;
	// vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	compileShader(vertex, vertexCode);
	checkCompileErrors(vertex, "VERTEX");
	// fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	fragmentCode = processDirectives(fragmentCode);
	compileShader(fragment, fragmentCode);
	checkCompileErrors(fragment, "FRAGMENT");

	// tesselation control Shader
    tessControl = glCreateShader(GL_TESS_CONTROL_SHADER);
	compileShader(tessControl, tessControlCode);
    checkCompileErrors(tessControl, "TESS_CONTROL");
	// tesselation evaluation Shader
    tessEval = glCreateShader(GL_TESS_EVALUATION_SHADER);
	compileShader(tessEval, tessEvalCode);
    checkCompileErrors(tessEval, "TESS_EVALUATION");

	// shader Program
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	if (tessControlPath != nullptr)
		glAttachShader(ID, tessControl);
	if (tessEvalPath != nullptr)
		glAttachShader(ID, tessEval);
	glLinkProgram(ID);
	checkCompileErrors(ID, "PROGRAM");
	// delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(vertex);
	glDeleteShader(fragment);

}

void Shader::compileShaders(const char* computePath)
{
	// 1. retrieve the vertex/fragment source code from filePath
	std::string computeCode;
	std::ifstream cShaderFile;
	// ensure ifstream objects can throw exceptions:
	cShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		cShaderFile.open(computePath);
		std::stringstream cShaderStream;
		// read file's buffer contents into streams
		cShaderStream << cShaderFile.rdbuf();
		// close file handlers
		cShaderFile.close();
		// convert stream into string
		computeCode = cShaderStream.str();
	}
	catch (std::ifstream::failure& e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what() << std::endl;
	}
	// 2. compile shaders
	unsigned int compute;
	// compute Shader
	compute = glCreateShader(GL_COMPUTE_SHADER);
	computeCode = processDirectives(computeCode);
	compileShader(compute, computeCode);
	checkCompileErrors(compute, "COMPUTE");

	// shader Program
	ID = glCreateProgram();
	glAttachShader(ID, compute);
	glLinkProgram(ID);
	checkCompileErrors(ID, "PROGRAM");
	// delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(compute);

}

//Shorthand for dispatch compute with some default parameter values
void Shader::dispatch(unsigned int x, unsigned int y, unsigned int z) const {
	glDispatchCompute(x, y, z);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT); // Ensure correct ordering of operations
}

void Shader::use()
{
	glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const
{
	glUniform1i(getUniformLocation(name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(getUniformLocation(name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(getUniformLocation(name.c_str()), value);
}

void Shader::setVec2(const std::string& name, const glm::vec2& vec) const
{
	glUniform2fv(getUniformLocation(name.c_str()), 1, &vec[0]);
}

void Shader::setVec3(const std::string& name, const glm::vec3& vec) const
{
	glUniform3fv(getUniformLocation(name.c_str()), 1, &vec[0]);
}

void Shader::setVec4(const std::string& name, const glm::vec4& vec) const
{
	glUniform4fv(getUniformLocation(name.c_str()), 1, &vec[0]);
}

void Shader::setMat2(const std::string& name, glm::mat4& mat) const
{
	glUniformMatrix2fv(getUniformLocation(name.c_str()), 1, GL_FALSE, &mat[0][0]);
}


void Shader::setMat3(const std::string& name, glm::mat4& mat) const
{
	glUniformMatrix3fv(getUniformLocation(name.c_str()), 1, GL_FALSE, &mat[0][0]);
}


void Shader::setMat4(const std::string& name, glm::mat4& mat) const
{
	glUniformMatrix4fv(getUniformLocation(name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

inline void Shader::setTexture2D(const std::string& textureName)
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char* data = stbi_load(textureName.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}

Shader::~Shader()
{
	glDeleteProgram(ID);
}

int Shader::getUniformLocation(const std::string& name) const
{
	if (uniformLocationCache.find(name) != uniformLocationCache.end()) 
		return uniformLocationCache[name];

	int location = glGetUniformLocation(ID, name.c_str());
	uniformLocationCache[name] = location;

	return location;
}

// utility function for checking shader compilation/linking errors.
   // ------------------------------------------------------------------------
void Shader::checkCompileErrors(GLuint shader, std::string type)
{
	GLint success;
	GLchar infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}

std::string Shader::loadShader(const std::string& filepath) {
	std::ifstream file(filepath);
	std::stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
}

std::string Shader::processDirectives(std::string& result) {
	processIncludes(result);
	processLoops(result);
	return result;
}

std::string Shader::processIncludes(std::string& result) {
	std::regex includeRegex("#pragma include \"([^\"]+)\"");
	std::smatch match;

	while (std::regex_search(result, match, includeRegex)) {
		std::string includeFile = match[1].str();
		std::string includeSource = loadShader(includeFile);
		result.replace(match.position(), match.length(), includeSource);
	}
	return result;
}

std::string Shader::processLoops(std::string& result) {
	try {
		std::regex forLoopRegex("#pragma FOR_LOOP\\((\\d+)\\)((.|\\n)*?)#pragma END_FOR_LOOP");
		std::smatch match2;

		while (std::regex_search(result, match2, forLoopRegex)) {
			int loopCount = std::stoi(match2[1]);
			std::string loopContent = match2[2];
			std::string replacement;
			for (int i = 0; i < loopCount; ++i) {
				std::string modifiedLoopContent = loopContent;
				size_t found = modifiedLoopContent.find("[i]");
				// To jest do obslugi identyfikatorow sztucznych petli w shaderach - moze potem lepiej zmienic na cos innego niz 'i'
				while (found != std::string::npos) {
					modifiedLoopContent.replace(found, 3, "[" + std::to_string(i) + "]");
					found = modifiedLoopContent.find("[i]", found + 1);
				}
				found = modifiedLoopContent.find("[i");
				while (found != std::string::npos) {
					modifiedLoopContent.replace(found, 3, "[" + std::to_string(i));
					found = modifiedLoopContent.find("[i", found + 1);
				}
				replacement += modifiedLoopContent;
			}
			// Replace the loop section with the generated content
			result = std::regex_replace(result, forLoopRegex, replacement, std::regex_constants::format_first_only);
		}
		return result;
	}
	catch (std::exception e) {
		std::cout << "ERROR: PROCESS LOOPS REGEX ERROR: \n\n" << e.what() << "\n\n";
	}
}

void Shader::compileShader(GLuint shader, const std::string& source) {
	const char* sourceCStr = source.c_str();
	glShaderSource(shader, 1, &sourceCStr, nullptr);
	glCompileShader(shader);
}