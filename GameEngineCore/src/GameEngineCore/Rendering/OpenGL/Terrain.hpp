#pragma once

#include "Model.hpp"

namespace GameEngine {
	class Terrain {
	private:
		const int SIZE = 700;
		const int VERTICES_COUNT = 128;

		float x, z;
		
		std::unique_ptr<Model> model = nullptr;
		
		std::shared_ptr<ShaderProgram> shader = nullptr;
	public:
		Terrain(int gridX, int gridZ, Texture texture);

		std::unique_ptr<Model> generateTerrain(Texture texture);

		void draw(glm::mat4 viewAndProjectionMatrix);

		void setShader(std::shared_ptr<ShaderProgram> shader) {
			this->shader = shader;
		}
		
	};
}