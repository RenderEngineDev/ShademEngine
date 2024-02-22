#pragma once

#include <string>
#include "glm/glm.hpp"

namespace ObjectAttributes {

	struct Common {
		std::string name;
		glm::vec3 position;
		glm::vec3 scale;
		glm::vec3 rotation;

		Common(glm::vec3 position = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f), glm::vec3 rotation = glm::vec3(0.0f), std::string name = "Object") : position(position), scale(scale), rotation(rotation), name(name) {};
	private:
		virtual void makeMePolymorphic() { };
	};

	struct Sphereous : public Common {
		float radius = 1;

		virtual void makeMePolymorphic() override { };
		Sphereous(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation, float radius, std::string name = "Sphere") : radius(radius), Common(position, scale, rotation, name) {};
		Sphereous(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation, std::string name = "Sphere") : Common(position, scale, rotation, name) {};
		Sphereous(glm::vec3 position, glm::vec3 scale, std::string name = "Sphere") : Common(position, scale, glm::vec3(0.0f), name) {};
	};

	namespace CubeMarching {
		struct CommonCubeMarching : public Common {
			bool lodOn = false;
			float isoValue = 1.0f;

			glm::vec3 gridSize = glm::vec3(20.0f);

			virtual void makeMePolymorphic() override { };
			// TODO: porz¹dek z konstruktorami, ale po ogarniêciu innych atrybutów
			CommonCubeMarching(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation, glm::vec3 gridSize, float isoValue = 1.0f, bool lodOn = false, std::string name = "CubeMarching") : gridSize(gridSize), isoValue(isoValue), lodOn(lodOn), Common(position, scale, rotation, name) {};
			CommonCubeMarching(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation, std::string name = "CubeMarching") : Common(position, scale, rotation, name) {};
		};	

		struct Noise : public CommonCubeMarching {
			float frequency;
			float amplitude;
			float lacunarity;
			float persistence;
			float noiseScale = 1.0f;
			glm::vec3 offset = glm::vec3(0.0f);

			virtual void makeMePolymorphic() override { };
			Noise(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation, glm::vec3 gridSize, float isoValue, float noiseScale, float frequency = 1.0f, float amplitude = 1.0f, float lacunarity = 2.0f, float persistence = 1 / 2.0f, bool lodOn = false, std::string name = "Noise (CM)")
				: noiseScale(noiseScale), frequency(frequency), amplitude(amplitude), lacunarity(lacunarity), persistence(persistence), CommonCubeMarching(position, scale, rotation, gridSize, isoValue, lodOn, name) {};
		};

		struct Sphere : public CommonCubeMarching {
			float radius;

			virtual void makeMePolymorphic() override { };
			Sphere(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation, glm::vec3 gridSize, float isoValue = 1.0f, float radius = 1.0f, std::string name = "Sphere (CM)")
				: radius(radius), CommonCubeMarching(position, scale, rotation, gridSize, isoValue, false, name) {};
		};
	}

	namespace Lights {
		struct Light : public Common {
			glm::vec3 ambient;
			glm::vec3 diffuse;
			glm::vec3 specular;

			Light(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation,
				glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, std::string name = "Light")
				: ambient(ambient), diffuse(diffuse), specular(specular),
				Common(position, scale, rotation, name) {};
		};

		struct PositionLight : public Light {

			float constant;
			float linear;
			float quadratic;

			PositionLight(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation, 
				glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
				float constant, float linear, float quadratic, std::string name = "PositionLight") 
				: constant(constant), linear(linear), quadratic(quadratic),
				Light(position, scale, rotation, ambient, diffuse, specular, name) {};
		};	

		struct SpotLight : public Light {
			float cutOff;
			float outerCutOff;

			float constant;
			float linear;
			float quadratic;

			glm::vec3 direction;
			SpotLight(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation,
				glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
				float cutOff, float outerCutOff, float constant, float linear, float quadratic, 
				std::string name = "SpotLight")
				: direction(direction), cutOff(cutOff), outerCutOff(outerCutOff), constant(constant), linear(linear), quadratic(quadratic),
				Light(position, scale, rotation, ambient, diffuse, specular, name) {};
		};	

		struct DirectionalLight : public Light {
			glm::vec3 direction;

			DirectionalLight(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation,
				glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
				std::string name = "DirectionalLight")
				: direction(direction), Light(position, scale, rotation, ambient, diffuse, specular, name) {};
		};	

		struct PointLight : public Light {
			float constant;
			float linear;
			float quadratic;

			PointLight(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation,
				glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
				float constant, float linear, float quadratic, std::string name = "PointLight")
				: constant(constant), linear(linear), quadratic(quadratic),
				Light(position, scale, rotation, ambient, diffuse, specular, name) {};
		};	
	}
}
