#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <stb_image.h>

namespace Shaders {
	static const char* getCommonFragmentShaderPath() {
		return "../Shadem/Shaders/BasicShader/Fshader.shader";
	}
	static const char* getCommonVertexShaderPath() {
		return "../Shadem/Shaders/BasicShader/Vshader.shader";
	}

	static const char* getRmSphereFragmentShaderPath() {
		return "../Shadem/Shaders/RMShaders/SphereFshader.shader";
	}
	static const char* getRmVertexShaderPath() {
		return "../Shadem/Shaders/RMShaders/Vshader.shader";
	}

	static const char* getLightFragmentShaderPath() {
		return "../Shadem/Shaders/BasicShader/FLightShader.shader";
	}
	static const char* getLightVertexShaderPath() {
		return "../Shadem/Shaders/BasicShader/VLightShader.shader";
	}

	static const char* getComputeShaderPath() {
		return "../Shadem/Shaders/ComputeShader/Cshader.shader";
	}
	static const char* getComputeVertexShaderPath() {
		return "../Shadem/Shaders/ComputeShader/Vshader.shader";
	}
	static const char* getComputeFragmentShaderPath() {
		return "../Shadem/Shaders/ComputeShader/Fshader.shader";
	}

	static const char* getCMFragmentShaderPath() {
		return "../Shadem/Shaders/CMShaders/FshaderCM.shader";
	}
	static const char* getCMVertexShaderPath() {
		return "../Shadem/Shaders/CMShaders/VshaderCM.shader";
	}
	static const char* getTesselationTCSShaderPath() {
		return "../Shadem/Shaders/CMShaders/tessellationTCS.shader";
	}
	static const char* getTesselationTESShaderPath() {
		return "../Shadem/Shaders/CMShaders/tessellationTES.shader";
	}
	static const char* getGeometryShaderCM3DPath() {
		return "../Shadem/Shaders/CMShaders/geometryShaderCM3D.shader";
	}
	static const char* getGeometryShaderCM2DPath() {
		return "../Shadem/Shaders/CMShaders/geometryShaderCM2D.shader";
	}
	static const char* getGeometryShaderTablesPath() {
		return "../Shadem/Shaders/CMShaders/CubeMarchingTables.shader";
	}
	static const char* getCubeMarchingShaderPath() {
		return "../Shadem/Shaders/CMShaders/CubeMarching.shader";
	}
	static const char* getNoiseShaderPath() {
		return "../Shadem/Shaders/CMShaders/Noise.shader";
	}

}

class Shader
{
	private:
		mutable std::unordered_map<std::string, int> uniformLocationCache = std::unordered_map<std::string, int>();

	public: 
		// the program ID
		unsigned int ID;
		unsigned int texture;

		// constructor reads and builds the shader
		Shader() {};
		Shader(std::initializer_list<const std::string> shaderPaths);
		void compileShaders(const char* vertexPath, const char* fragmentPath);
		void compileShaders(const char* vertexPath, const char* fragmentPath, const char* geometryPath);
		void compileShaders(const char* vertexPath, const char* fragmentPath, const char* tessControlPath, const char* tessEvalPath);
		void compileShaders(const char* computePath);
		void dispatch(unsigned int x, unsigned int y, unsigned int z) const;

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

		~Shader();

private:
		
		int getUniformLocation(const std::string& name) const;

		void checkCompileErrors(GLuint shader, std::string type);
		std::string loadShader(const std::string& filepath);
		std::string processDirectives(std::string& source);
		std::string processIncludes(std::string& source);
		std::string processLoops(std::string& source);
		void compileShader(GLuint shader, const std::string& source);
};