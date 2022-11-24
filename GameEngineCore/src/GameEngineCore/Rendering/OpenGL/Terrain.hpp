#pragma once

#include "Model.hpp"

namespace GameEngine {
	class Terrain {
	private:
		const int SIZE = 700;
		float x, z;
		
		const float HEIGHT_SCALE = 64.0f / 256.0f;
		const float HEIGHT_SHIFT = 24.f;

		float getHeight(int x, int y, unsigned char* heightMap, int heightMapHeight);
		glm::vec3 calculateNormal(int x, int y, unsigned char* heightMap, int heightMapHeight);
		std::unique_ptr<Model> model = nullptr;
		
		std::shared_ptr<ShaderProgram> shader = nullptr;
	public:
		Terrain(int gridX, int gridZ, Texture texture, const char* heightMapLocation);

		std::unique_ptr<Model> generateTerrain(Texture texture, const char* heightMapLocation);

		void draw(glm::mat4 viewAndProjectionMatrix);

		void setShader(std::shared_ptr<ShaderProgram> shader) {
			this->shader = shader;
		}
		
	};
}