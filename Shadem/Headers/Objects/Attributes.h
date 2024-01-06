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

	struct CubeMarching : public Common {
		float radius;
		float isoValue = 1.0f;

		// FIXME: Dodane tymczasowo na potrzeby prezentacji - potem do zmiany
		float frequency;
		float amplitude;
		float lacunarity;
		float persistence;

		glm::vec3 gridSize = glm::vec3(20.0f);

		virtual void makeMePolymorphic() override { };
		// TODO: porz¹dek z konstruktorami, ale po ogarniêciu innych atrybutów
		CubeMarching(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation, glm::vec3 gridSize, float isoValue, float frequency = 1.0f, float amplitude = 1.0f, float lacunarity = 2.0f, float persistence = 1 / 2.0f, std::string name = "CubeMarching") : gridSize(gridSize), isoValue(isoValue), frequency(frequency), amplitude(amplitude), lacunarity(lacunarity), persistence(persistence), Common(position, scale, rotation, name) {};
		CubeMarching(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation, glm::vec3 gridSize, float isoValue = 1.0f, float radius = 1.0f, std::string name = "CubeMarching") : gridSize(gridSize), isoValue(isoValue), radius(radius), Common(position, scale, rotation, name) {};
		CubeMarching(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation, std::string name = "CubeMarching") : Common(position, scale, rotation, name) {};
	};	

}
//TODO: Czy robiæ atrybuty na ka¿dy typ obiektu?
//struct NoiseAttributes : public CubeMarchingAttributes{};
//struct SphereAttributes : public CubeMarchingAttributes{};
