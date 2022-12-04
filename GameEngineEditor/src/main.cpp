#include <iostream>
#include <memory>

#include <GameEngineCore/Application.hpp>
#include <GameEngineCore/Scene.hpp>
#include <GameEngineCore/ObjectManager.hpp>
#include <GameEngineCore/ShaderManager.hpp>
#include <GameEngineCore/Enums.hpp>
#include <GameEngineCore/Vertex.hpp>
#include <GameEngineCore/InterfaceManager.hpp>

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>


std::vector<GameEngine::Vertex> lightCubeVertices = {
           GameEngine::Vertex(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
           GameEngine::Vertex(glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
           GameEngine::Vertex(glm::vec3(1.0f, 1.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
           GameEngine::Vertex(glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
           GameEngine::Vertex(glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
           GameEngine::Vertex(glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
           GameEngine::Vertex(glm::vec3(1.0f,  1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
           GameEngine::Vertex(glm::vec3(-1.0f,  1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
};

std::vector<unsigned int> indices = {
            0,2,1,
            0,3,2,

            1,2,6,
            6,5,1,

            4,5,6,
            6,7,4,

            2,3,6,
            6,3,7,

            0,7,3,
            0,4,7,

            0,1,5,
            0,5,4
    };


const char* terrainTextureLocation = "../../GameEngineCore/assets/ground.jpg";
const char* heightMapLocation = "../../GameEngineCore/assets/heightMap.png";

glm::vec3 sunLightDirection = { -0.2f, -1.0f, -0.3f };

class MyApp : public GameEngine::Application {
	
	void initScene() override {
        GameEngine::Scene::addTerrain("Terrain1", 0, 0, terrainTextureLocation, nullptr);
        GameEngine::Scene::addTerrain("Terrain2", 1, 0, terrainTextureLocation, nullptr);
        GameEngine::Scene::addObject("Raiden",
            "../../GameEngineCore/assets/models/raiden-shogun-genshin-impact/raiden_shogun.fbx",
            glm::vec3(50.f, 3.f, 50.f),
            glm::vec3(0.8f, 0.8f, 0.8f),
            0.f,
            GameEngine::ShaderType::LIGHTING_TEXTURE
        );
        GameEngine::Scene::addObject("Paimon",
            "../../GameEngineCore/assets/models/paimon/paimon.obj",
            glm::vec3(51.f, 4.f, 50.f),
            glm::vec3(0.1f, 0.1f, 0.1f),
            0.f,
            GameEngine::ShaderType::LIGHTING_TEXTURE
        );
        GameEngine::Scene::addObject("LightCube",
            lightCubeVertices, 
            indices,
            std::vector<const char*>(),
            glm::vec3(51.f, 4.f, 51.f),
            glm::vec3(0.3f, 0.3f, 0.3f),
            0.f,
            GameEngine::ShaderType::SIMPLE
        );

        GameEngine::Scene::addCamera(
            glm::vec3(50.0f, 10.0f, 55.0f),
            glm::vec3(0.f, 0.f, 0.f),
            GameEngine::ProjectionType::Perspective
        );


        GameEngine::Scene::addLight(GameEngine::LightType::DIRECTION, glm::vec3(0.0, 0.0, 0.0), sunLightDirection);
        GameEngine::Scene::addLight(GameEngine::LightType::POINT, GameEngine::ObjectManager::getObject("LightCube")->getPosition());
	
    
        GameEngine::InterfaceManager::addCombo(
            "ActiveObject",
            GameEngine::ObjectManager::getObjectNames(),
            GameEngine::ObjectManager::getObjectNames()[0],
            [](std::string name) {
                GameEngine::Scene::updateActiveObjectName(name);
            }
        );

        GameEngine::InterfaceManager::addCombo(
            "Transform",
            std::vector<std::string>({ "Move", "Rotate", "Scale" }),
            std::string("Move"),
            [](std::string name) {
                GameEngine::Scene::updateActiveTransform(name);
            }
        );

    }

    void update() override {
        
        
    }

};

int main() {
	
	auto myApp = std::make_unique<MyApp>();
	int returnCode = myApp->start(1024, 768, "Shinzo wo Sasageyo!");
	


	return returnCode;
	
}