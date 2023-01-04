#pragma once

#include <string> 
#include <vector>
#include <glm/vec3.hpp>
#include <GameEngineCore/Vertex.hpp>
#include <GameEngineCore/Enums.hpp>
#include <GameEngineCore/ShaderManager.hpp>


namespace GameEngine {

	

	enum class ShaderType;

	

	

	static class Scene {
	public:

		static void addObject(std::string name, std::string modelPath, glm::vec3 position, glm::vec3 scalation, glm::vec3 rotation, ShaderType shader, DrawType drawType = DrawType::Triangles);

		static void addObject(
			std::string name, 
			std::vector<Vertex> vertices, 
			std::vector<unsigned int> indices,
			std::vector<const char*> textures,
			glm::vec3 position, 
			glm::vec3 scalation, 
			glm::vec3 rotation,
			ShaderType shader,
			DrawType drawType = DrawType::Triangles
		);

		static void clear();
		static void removeObject(std::string name);

		static void addTerrain(std::string name, int gridX, int gridZ, const char* textureLocation, const char* heightMapLocation);
		static void removeTerrain(std::string name);

		static void addCamera(glm::vec3 position, glm::vec3 rotation, ProjectionType type);

		//void getObject(std::string name);

		static void addLight(LightType type, glm::vec3 vec, glm::vec3 diffuseColor, glm::vec3 specularColor, std::string objectName = "",
			float cutOff = 12.5f, float outerCutOff = 17.5f, glm::vec3 specDir = glm::vec3(1.0f, 1.0f, 1.0f));
		static void render();

		static std::vector<LightSource> getPointLights() { return pointLights; }
		static void setPointLight(short index, glm::vec3 value) { 
			auto pointLight = pointLights[index];
			pointLight.position = value;
			pointLights[index] = pointLight;
		}

		static void updatePointLightDiffuseColor(short index, glm::vec3 value) {
			auto pointLight = pointLights[index];
			pointLight.diffuseColor = value;
			pointLights[index] = pointLight;
		}
		

	private:
		static std::vector<LightSource> dirLights;
		static std::vector<LightSource> pointLights;
		static std::vector<LightSource> spotLights;
	};
}