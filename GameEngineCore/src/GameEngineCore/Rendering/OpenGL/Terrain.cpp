#include "Terrain.hpp"
#include <SOIL/SOIL.h>
#include <iostream>

namespace GameEngine {
	Terrain::Terrain(int gridX, int gridZ, Texture texture, const char* heightMapLocation) {
		this->x = gridX;
		this->z = gridZ;
		model = generateTerrain(texture, heightMapLocation);
	}

	void Terrain::draw(glm::mat4 viewAndProjectionMatrix) {
		shader->bind();
		shader->setMatrix4("viewAndProjectionMatrix", viewAndProjectionMatrix);
		shader->setMatrix4("transformMatrix", glm::mat4(1.f));
		model->draw(shader);
		
	}

	std::unique_ptr<Model> Terrain::generateTerrain(Texture texture, const char* heightMapLocation) {
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		int width = 0;
		int height = 0;
		int channel;
		unsigned char* heightMap = SOIL_load_image(heightMapLocation, &width, &height, 0, SOIL_LOAD_RGB);
		if (!heightMap) {
			/*LOG_CRITICAL("Image load failed: {0} {1}", _fileLocation, SOIL_last_result());*/
			return nullptr;
		}
		const int VERTICES_COUNT = height;

		for (int i = 0; i < VERTICES_COUNT; i++) {
			for (int j = 0; j < VERTICES_COUNT; j++) {
				glm::vec3 position;
				glm::vec3 normal;
				glm::vec2 texCoords;

				position.x = (float)j / ((float)VERTICES_COUNT - 1) * SIZE;
				position.y = getHeight(j, i, heightMap, height);
				position.z = (float)i / ((float)VERTICES_COUNT - 1) * SIZE;



				normal = calculateNormal(j, i, heightMap, height);

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

	float Terrain::getHeight(int x, int y, unsigned char* heightMap, int heightMapHeight) {
		if (x < 0 || x >= heightMapHeight || y < 0 || y >= heightMapHeight) {
			return 0;
		}
			
		unsigned char* pixel = heightMap + (y * heightMapHeight + x) * 3;
		unsigned char height = pixel[0];

		return (int)height * HEIGHT_SCALE - HEIGHT_SHIFT;
	}

	glm::vec3 Terrain::calculateNormal(int x, int y, unsigned char* heightMap, int heightMapHeight) {
		unsigned char* pixel = heightMap + (y * heightMapHeight + x) * 3;
		unsigned char height = pixel[0];
		float heightL = getHeight(x - 1, y, heightMap, heightMapHeight);
		float heightR = getHeight(x + 1, y, heightMap, heightMapHeight);
		float heightD = getHeight(x, y - 1, heightMap, heightMapHeight);
		float heightU = getHeight(x, y + 1, heightMap, heightMapHeight);
		glm::vec3 normal = glm::normalize(glm::vec3(heightL - heightR, 2.f, heightD - heightU ));

		return normal;


	}
}