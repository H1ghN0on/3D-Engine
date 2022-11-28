#pragma once

#include <map>

#include "Object.hpp"
#include "Terrain.hpp"

namespace GameEngine {
	
	
	class ObjectManager {
	private:

		ObjectManager() {};
		
		static std::map<std::string, Object*> objects;
		static std::map<std::string, Terrain*> terrains;
	public:

		static std::shared_ptr<ObjectManager> getInstance() {
			static std::shared_ptr<ObjectManager> s{ new ObjectManager };
			return s;
		}

		ObjectManager(const ObjectManager&) = delete;
		ObjectManager& operator=(ObjectManager&) = delete;

		
		unsigned int addObject(std::string name, Object* obj);
		unsigned int removeObject(std::string name);
		bool isObjectExists(std::string name);

		Object* getObject(std::string name);


		Terrain* getTerrain(std::string name);
		unsigned int addTerrain(std::string name, Terrain* obj);
		unsigned int removeTerrain(std::string name);
		bool isTerrainExists(std::string name);

		std::map<std::string, Object*> getObjects() const {
			return objects;
		}

		std::map<std::string, Terrain*> getTerrains() const {
			return terrains;
		}
	};
}