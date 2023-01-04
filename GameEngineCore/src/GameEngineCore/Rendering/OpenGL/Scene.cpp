#include <GameEngineCore/Scene.hpp>
#include <GameEngineCore/ObjectManager.hpp>


#include <iostream>

namespace GameEngine {

	std::vector<LightSource> Scene::dirLights = std::vector<LightSource>();
	std::vector<LightSource> Scene::spotLights = std::vector<LightSource>();
	std::vector<LightSource> Scene::pointLights = std::vector<LightSource>();

	void Scene::addObject(std::string name, std::string modelPath, glm::vec3 position, glm::vec3 scalation, glm::vec3 rotation, ShaderType shader, DrawType drawType) {
		Object* obj = new Object(modelPath.c_str(), position, scalation, rotation);
		ObjectManager::addObject(name, obj);
		ObjectManager::getObject(name)->setShader(ShaderManager::get(shader));
		ObjectManager::getObject(name)->setMaterial(ShaderMaterial::NONE);
		ObjectManager::getObject(name)->setDrawType(drawType);
	}

	void Scene::clear() {
		for (auto& obj : ObjectManager::getObjects()) {
			ObjectManager::removeObject(obj.first);
		}
		for (auto& terr : ObjectManager::getTerrains()) {
			ObjectManager::removeTerrain(terr.first);
		}

		dirLights.clear();
		pointLights.clear();
	}

	void Scene::addObject(
		std::string name,
		std::vector<Vertex> vertices,
		std::vector<unsigned int> indices,
		std::vector<const char*> textureLocations,
		glm::vec3 position,
		glm::vec3 scalation,
		glm::vec3 rotation,
		ShaderType shader,
		DrawType drawType
	) {
		std::vector<Texture> textures = std::vector<Texture>();
		if (textureLocations.size() > 0) {
			textures.push_back(Texture(
				textureLocations[0],
				Texture::Type::Diffusal,
				Texture::WrappingMode::Repeat,
				Texture::MipmapFilterMode::LinearLinear
			));

			if (textureLocations.size() > 1) {
				textures.push_back(Texture(
					textureLocations[1],
					Texture::Type::Specular,
					Texture::WrappingMode::Repeat,
					Texture::MipmapFilterMode::LinearLinear
				));
			}
		}
		

		

		Object* obj = new Object(vertices, indices, textures, position, scalation, rotation);
		ObjectManager::addObject(name, obj);
		ObjectManager::getObject(name)->setShader(ShaderManager::get(shader));
		ObjectManager::getObject(name)->setDrawType(drawType);
	}

	void Scene::removeObject(std::string name) {
		ObjectManager::removeObject(name);
	}

	void Scene::addLight(LightType type, glm::vec3 vec, glm::vec3 diffuseColor, glm::vec3 specularColor, std::string objectName, float cutOff,
		float outerCutOff, glm::vec3 specDir) {
		switch (type) {
			case LightType::DIRECTION: {
				dirLights.push_back(LightSource(type, vec, diffuseColor, specularColor, objectName));
				break;
			}
			case LightType::POINT: {
				pointLights.push_back(LightSource(type, vec, diffuseColor, specularColor, objectName));
				if (objectName != "") {
					ObjectManager::getObject(objectName)->setLightIndex(pointLights.size() - 1);
				}
				break;
			}
			case LightType::SPOT: {
				spotLights.push_back(LightSource(type, vec, diffuseColor, specularColor, objectName, cutOff, outerCutOff, specDir));
				if (objectName != "") {
					ObjectManager::getObject(objectName)->setLightIndex(spotLights.size() - 1);
				}
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
		if (pointLights.size()) {
			ShaderManager::setLightInfo(
				dirLights,
				/*camera->getPosition(),
				camera->getFront(),*/
				pointLights,
				spotLights
			);
		}
		

		ShaderManager::setViewAndProjectionMatrix(ObjectManager::getCamera()->update());

		bool hasTextures = false;

		for (auto& [key, object] : ObjectManager::getObjects()) {
			Renderer::setDrawType(object->getDrawType());

			ShaderManager::setMaterial(object->getMaterial());

			object->draw();
			Renderer::setDrawType(DrawType::Triangles);
		}

		for (auto& [key, terrain] : ObjectManager::getTerrains()) {
			terrain->draw();
		}
	}
}