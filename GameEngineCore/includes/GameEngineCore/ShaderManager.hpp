#pragma once

#include <map>
#include <string>
#include <memory>

#include <GameEngineCore/Rendering/OpenGL/ShaderProgram.hpp>

namespace GameEngine {

	typedef std::map<char*, std::pair<ShaderProgram::PropertyTypes, std::any>> shader_property;

	enum class ShaderType {
		LIGHTING, SIMPLE, TERRAIN, LIGHTING_TEXTURE,
	};

	enum class ShaderMaterial {
		EMERALD,
		JADE,
		PEARL,
		GOLD,
		RUBBER,
	};

	static class ShaderManager {

	public:
		static unsigned int init();
		static std::shared_ptr<ShaderProgram> get(ShaderType type);
		static void setMaterial(ShaderMaterial);
		static void setLightInfo(
			std::vector<glm::vec3> dirLightDirection,
			//glm::vec3 spotLightPosition,
			//glm::vec3 spotLightDirection,
			std::vector<glm::vec3> pointLightPositions
		);

		static void setViewAndProjectionMatrix(glm::mat4 mat);

	private:

		static std::map<ShaderType, std::shared_ptr<ShaderProgram>> shaders;


	};
}