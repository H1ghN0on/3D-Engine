#pragma once

#include <string> 
#include <vector>
#include <glm/vec3.hpp>
#include <GameEngineCore/Vertex.hpp>
#include <GameEngineCore/Enums.hpp>



namespace GameEngine {

	enum class ShaderType;

	enum class LightType {
		POINT, SPOT, DIRECTION
	};

	static class Scene {
	public:

		static void addObject(std::string name, std::string modelPath, glm::vec3 position, glm::vec3 scalation, float rotation, ShaderType shader);

		static void addObject(
			std::string name, 
			std::vector<Vertex> vertices, 
			std::vector<unsigned int> indices,
			std::vector<const char*> textures,
			glm::vec3 position, 
			glm::vec3 scalation, 
			float rotation,
			ShaderType shader
		);
		static void removeObject(std::string name);

		static void addTerrain(std::string name, int gridX, int gridZ, const char* textureLocation, const char* heightMapLocation);
		static void removeTerrain(std::string name);

		static void addCamera(glm::vec3 position, glm::vec3 rotation, ProjectionType type);

		//void getObject(std::string name);

		static void addLight(LightType type, glm::vec3 position, glm::vec3 direction = glm::vec3(0.0f, 0.0f, 0.0f));
		static void render();

		static std::string getActiveObjectName() { return activeObjectName; }
		static void updateActiveObjectName(std::string name) { activeObjectName = name; }
	private:
		static std::vector<glm::vec3> dirLights;
		static std::vector<glm::vec3> pointLights;
		static std::string activeObjectName;
	};
}