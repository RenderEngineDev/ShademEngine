#pragma once

#include "Objects/Object.h"
#include "Objects/ObjectTypes.h"


namespace Lights {

	class Light : public Object {
	public:
		static std::vector<Lights::Light*> lights;
		ObjectTypes::LightObjectType::Type type;
		ObjectAttributes::Lights::Light* attributes;
		
		Light() {};
		Light(ObjectAttributes::Lights::Light* attributes, ObjectTypes::LightObjectType::Type type, std::initializer_list<const std::string> filePaths);
		virtual void draw(Camera::Camera& camera) override;
		virtual void update(Camera::Camera& camera) override;
		virtual void setupMesh() override;

		template<class ClassType, class ObjectType>
		static std::vector<ClassType*> getLightsByType(ObjectType type) {
			std::vector<ClassType*> outputLights;
			for (Light* light : Lights::Light::lights) {
				if (light->type == type) {
					outputLights.push_back(dynamic_cast<ClassType*>(light));
				}
			}
			return outputLights;
		}
		static void setShaderLights(Shader* shader);
	};

	class PositionLight : public Light {
	public:
		PositionLight(ObjectAttributes::Lights::PositionLight* attributes, ObjectTypes::LightObjectType::Type lightType, std::initializer_list<const std::string> filePaths) : Light(attributes, lightType, filePaths) {
		};

		virtual ObjectAttributes::Lights::PositionLight* getAttributes() const override {
			return dynamic_cast<ObjectAttributes::Lights::PositionLight*>(attributes);
		}
	};

	class SpotLight : public Light {
	public:
		SpotLight(ObjectAttributes::Lights::SpotLight* attributes, ObjectTypes::LightObjectType::Type lightType, std::initializer_list<const std::string> filePaths) : Light(attributes, lightType, filePaths) {};

		virtual ObjectAttributes::Lights::SpotLight* getAttributes() const override {
			return dynamic_cast<ObjectAttributes::Lights::SpotLight*>(attributes);
		}
	};

	class DirectionalLight : public Light {
	public:
		DirectionalLight(ObjectAttributes::Lights::DirectionalLight* attributes, ObjectTypes::LightObjectType::Type lightType, std::initializer_list<const std::string> filePaths) : Light(attributes, lightType, filePaths) {
		};

		virtual ObjectAttributes::Lights::DirectionalLight* getAttributes() const override {
			return dynamic_cast<ObjectAttributes::Lights::DirectionalLight*>(attributes);
		}
	};

	class PointLight : public Light {
	public:
		PointLight(ObjectAttributes::Lights::PointLight* attributes, ObjectTypes::LightObjectType::Type lightType, std::initializer_list<const std::string> filePaths) : Light(attributes, lightType, filePaths) {
		};

		virtual ObjectAttributes::Lights::PointLight* getAttributes() const override {
			return dynamic_cast<ObjectAttributes::Lights::PointLight*>(attributes);
		}
	};
}