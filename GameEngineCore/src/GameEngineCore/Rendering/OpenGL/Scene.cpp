#include <GameEngineCore/Scene.hpp>
#include <GameEngineCore/ObjectManager.hpp>
#include <GameEngineCore/ShaderManager.hpp>

namespace GameEngine {

	std::vector<glm::vec3> Scene::dirLights = std::vector<glm::vec3>();
	std::vector<glm::vec3> Scene::pointLights = std::vector<glm::vec3>();

	void Scene::addObject(std::string name, std::string modelPath, glm::vec3 position, glm::vec3 scalation, glm::vec3 rotation, ShaderType shader) {
		Object* obj = new Object(modelPath.c_str(), position, scalation, rotation);
		ObjectManager::addObject(name, obj);
		ObjectManager::getObject(name)->setShader(ShaderManager::get(shader));
	}

	void Scene::addObject(
		std::string name,
		std::vector<Vertex> vertices,
		std::vector<unsigned int> indices,
		std::vector<const char*> textureLocations,
		glm::vec3 position,
		glm::vec3 scalation,
		glm::vec3 rotation,
		ShaderType shader
	) {
		std::vector<Texture> textures = std::vector<Texture>();
		for (auto& textureLocation : textureLocations) {
			textures.push_back(Texture(
				textureLocation,
				Texture::Type::Diffusal,
				Texture::WrappingMode::Repeat,
				Texture::MipmapFilterMode::LinearLinear
			));
		}


		Object* obj = new Object(vertices, indices, textures, position, scalation, rotation);
		ObjectManager::addObject(name, obj);
		ObjectManager::getObject(name)->setShader(ShaderManager::get(shader));
	}

	void Scene::removeObject(std::string name) {
		ObjectManager::removeObject(name);
	}

	void Scene::addLight(LightType type, glm::vec3 position, glm::vec3 direction) {
		switch (type) {
			case LightType::DIRECTION: {
				dirLights.push_back(direction);
				break;
			}
			case LightType::POINT: {
				pointLights.push_back(position);
				break;
			}
			case LightType::SPOT: {
				break;
			}
		}
	}

	void Scene::addTerrain(std::string name, int gridX, int gridZ, const char* textureLocation, const char* heightMapLocation) {
		Texture terrainTexture(
			textureLocation,
			Texture::Type::Diffusal,
			Texture::WrappingMode::Repeat,
			Texture::MipmapFilterMode::LinearLinear
		);

		Terrain* terrain = new Terrain(gridX, gridZ, terrainTexture, heightMapLocation);
		ObjectManager::addTerrain(name, terrain);
		ObjectManager::getTerrain(name)->setShader(ShaderManager::get(ShaderType::TERRAIN));
	}

	void Scene::removeTerrain(std::string name) {
		ObjectManager::removeTerrain(name);
	}

	void Scene::addCamera(glm::vec3 position, glm::vec3 rotation, ProjectionType type) {
		CameraObject* camera = new CameraObject(position, rotation, type);
		ObjectManager::addCamera(camera);
	}

	void Scene::render() {
		ShaderManager::setLightInfo(
			dirLights,
			/*camera->getPosition(),
			camera->getFront(),*/
			pointLights
		);

		ShaderManager::setViewAndProjectionMatrix(ObjectManager::getCamera()->update());


		for (auto& [key, object] : ObjectManager::getObjects()) {
			object->draw();
		}

		for (auto& [key, terrain] : ObjectManager::getTerrains()) {
			terrain->draw();
		}
	}
}