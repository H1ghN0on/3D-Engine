#pragma once

#include <map>

#include "Object.hpp"
#include "Terrain.hpp"
#include "CameraObject.hpp"

namespace GameEngine {
	
	
	class ObjectManager {
	private:
		
		static std::map<std::string, Object*> objects;
		static std::map<std::string, Terrain*> terrains;
		static CameraObject* camera;
	public:

		ObjectManager(const ObjectManager&) = delete;
		ObjectManager& operator=(ObjectManager&) = delete;

		static unsigned int addCamera(CameraObject* obj);
		static unsigned int addObject(std::string name, Object* obj);
		static unsigned int removeObject(std::string name);
		static bool isObjectExists(std::string name);

		static Object* getObject(std::string name);


		static Terrain* getTerrain(std::string name);
		static unsigned int addTerrain(std::string name, Terrain* obj);
		static unsigned int removeTerrain(std::string name);
		static bool isTerrainExists(std::string name);

		static std::map<std::string, Object*> getObjects() {
			return objects;
		}

		static std::map<std::string, Terrain*> getTerrains() {
			return terrains;
		}
		static CameraObject* getCamera() {
			return camera;
		}
	};
}