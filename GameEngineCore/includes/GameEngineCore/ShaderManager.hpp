#pragma once

#include <map>
#include <string>
#include <memory>

#include <GameEngineCore/Enums.hpp>
#include <GameEngineCore/Rendering/OpenGL/ShaderProgram.hpp>

namespace GameEngine {

	typedef std::map<char*, std::pair<ShaderProgram::PropertyTypes, std::any>> shader_property;

	enum class ShaderType {
		LIGHTING, SIMPLE, TERRAIN, LIGHTING_TEXTURE, SUN
	};

	enum class LightType {
		POINT, SPOT, DIRECTION
	};

	

	struct LightSource {
		glm::vec3 diffuseColor;
		glm::vec3 specularColor;
		glm::vec3 position;
		glm::vec3 direction;
		float cutOff;
		float outerCutOff;
		std::string objectName;
		LightType type;

		LightSource(LightType type, glm::vec3 vec, glm::vec3 diffuseColor, glm::vec3 specularColor, std::string objectName = "",
			float cutOff = 12.5f, float outerCutOff = 17.5f, glm::vec3 specDir = glm::vec3(1.0f, 1.0f, 1.0f)) {
			switch (type) {
			case LightType::POINT: {
				this->position = vec;
				this->diffuseColor = diffuseColor;
				this->specularColor = specularColor;
				this->objectName = objectName;
				this->type = LightType::POINT;
				break;

			}

			case LightType::DIRECTION: {
				this->direction = vec;
				this->diffuseColor = diffuseColor;
				this->specularColor = specularColor;
				this->objectName = objectName;
				this->type = LightType::DIRECTION;
				break;
			}

			case LightType::SPOT: {
				this->direction = specDir;
				this->diffuseColor = diffuseColor;
				this->specularColor = specularColor;
				this->objectName = objectName;
				this->type = LightType::SPOT;
				this->cutOff = cutOff;
				this->outerCutOff = outerCutOff;
				this->position = position;
				break;
			}
			}
		}
	};

	static class ShaderManager {

	public:
		static unsigned int init();
		static std::shared_ptr<ShaderProgram> get(ShaderType type);
		static void setMaterial(ShaderMaterial);
		static void setLightInfo(
			std::vector<LightSource> dirLightDirection,
			//glm::vec3 spotLightPosition,
			//glm::vec3 spotLightDirection,
			std::vector<LightSource> pointLightPositions,
			std::vector<LightSource> spotLightPositions
		);

		static void setViewAndProjectionMatrix(glm::mat4 mat);

		static void setMaterial();

	private:

		static std::map<ShaderType, std::shared_ptr<ShaderProgram>> shaders;


	};
}