#include "Terrain.hpp"


namespace GameEngine {
	Terrain::Terrain(int gridX, int gridZ, Texture texture) {
		this->x = gridX;
		this->z = gridZ;

		model = generateTerrain(texture);
	}

	void Terrain::draw(glm::mat4 viewAndProjectionMatrix) {
		shader->bind();
		shader->setMatrix4("viewAndProjectionMatrix", viewAndProjectionMatrix);
		shader->setMatrix4("transformMatrix", glm::mat4(1.f));
		model->draw(shader);
		
	}

	std::unique_ptr<Model> Terrain::generateTerrain(Texture texture) {
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		for (int i = 0; i < VERTICES_COUNT; i++) {
			for (int j = 0; j < VERTICES_COUNT; j++) {
				glm::vec3 position;
				glm::vec3 normal;
				glm::vec2 texCoords;

				position.x = (float)j / ((float)VERTICES_COUNT - 1) * SIZE;
				position.y = 0;
				position.z = (float)i / ((float)VERTICES_COUNT - 1) * SIZE;
				normal.x = 0;
				normal.y = 1;
				normal.z = 0;
				texCoords.x = (float)j / ((float)VERTICES_COUNT - 1);
				texCoords.y = (float)i / ((float)VERTICES_COUNT - 1);

				vertices.push_back(Vertex(position, normal, texCoords));
			}
		}
		for (int gz = 0; gz < VERTICES_COUNT - 1; gz++) {
			for (int gx = 0; gx < VERTICES_COUNT - 1; gx++) {
				int topLeft = (gz * VERTICES_COUNT) + gx;
				int topRight = topLeft + 1;
				int bottomLeft = ((gz + 1) * VERTICES_COUNT) + gx;
				int bottomRight = bottomLeft + 1;
				indices.push_back(topLeft);
				indices.push_back(bottomLeft);
				indices.push_back(topRight);
				indices.push_back(topRight);
				indices.push_back(bottomLeft);
				indices.push_back(bottomRight);
			}
		}

		return std::make_unique<Model>(vertices, indices, std::vector<Texture>({ texture }));
	}
}