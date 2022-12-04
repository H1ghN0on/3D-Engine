#include <GameEngineCore/ObjectManager.hpp>
#include <stdexcept>

#include <iostream>

namespace GameEngine {

	std::map<std::string, Object*> ObjectManager::objects = std::map<std::string, Object*>();
	std::map<std::string, Terrain*> ObjectManager::terrains = std::map<std::string, Terrain*>();
	CameraObject* ObjectManager::camera = nullptr;

	unsigned int ObjectManager::addObject(std::string name, Object* obj) {
		if (isObjectExists(name)) {
			return 1;
		}
		objects.insert(std::make_pair(name, obj));
		return 0;
	}


	unsigned int ObjectManager::removeObject(std::string name) {
		if (isObjectExists(name)) {
			objects.erase(name);
			return 0;
		}
		return 1;
	}

	bool ObjectManager::isObjectExists(std::string name) {
		auto object = objects.find(name);
		return object != objects.end();
	}

	Object* ObjectManager::getObject(std::string name) {
		if (isObjectExists(name)) {
			return objects[name];
		}
		return nullptr;
	}


	unsigned int ObjectManager::addTerrain(std::string name, Terrain* obj) {
		if (isTerrainExists(name)) {
			return 1;
		}
		terrains.insert(std::make_pair(name, obj));
		return 0;
	}


	unsigned int ObjectManager::addCamera(CameraObject* _camera) {
		if (camera != nullptr) {
			delete camera;
		}
		camera = _camera;
		return 0;
	}


	unsigned int ObjectManager::removeTerrain(std::string name) {
		if (isTerrainExists(name)) {
			terrains.erase(name);
			return 0;
		}
		return 1;
	}

	bool ObjectManager::isTerrainExists(std::string name) {
		auto terrain = terrains.find(name);
		return terrain != terrains.end();
	}

	Terrain* ObjectManager::getTerrain(std::string name) {
		if (isTerrainExists(name)) {
			return terrains[name];
		}
		return nullptr;
	}



}