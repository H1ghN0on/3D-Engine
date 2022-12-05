#include <iostream>
#include <memory>
#include <random>

#include <GameEngineCore/Application.hpp>
#include <GameEngineCore/Scene.hpp>
#include <GameEngineCore/ObjectManager.hpp>
#include <GameEngineCore/ShaderManager.hpp>
#include <GameEngineCore/Enums.hpp>
#include <GameEngineCore/Vertex.hpp>
#include <GameEngineCore/InterfaceManager.hpp>
#include <GameEngineCore/TransformManager.hpp>
#include <GameEngineCore/Renderer.hpp>
#include <map>

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

const float M_PI = 3.14f;

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


void addFigure(std::vector<GameEngine::Vertex> _vertices, std::vector<unsigned int> _indices, GameEngine::DrawType drawType = GameEngine::DrawType::Triangles) {
    GameEngine::Scene::addObject(
        "NRectangle",
        _vertices,
        _indices,
        std::vector<const char*>(),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        GameEngine::ShaderType::SIMPLE,
        drawType
    );
}

void addFigure(std::string name, std::vector<GameEngine::Vertex> _vertices, std::vector<unsigned int> _indices, glm::vec3 position, GameEngine::DrawType drawType = GameEngine::DrawType::Triangles) {
    GameEngine::Scene::addObject(
        name,
        _vertices,
        _indices,
        std::vector<const char*>(),
        position,
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        GameEngine::ShaderType::SIMPLE,
        drawType
    );
}

const char* terrainTextureLocation = "../../GameEngineCore/assets/ground.jpg";
const char* heightMapLocation = "../../GameEngineCore/assets/heightMap.png";

glm::vec3 sunLightDirection = { -0.2f, -1.0f, -0.3f };


void coursework() {
    GameEngine::Renderer::enableDepth(true);
    GameEngine::Scene::addTerrain("Terrain1", 0, 0, terrainTextureLocation, nullptr);
    GameEngine::Scene::addTerrain("Terrain2", 1, 0, terrainTextureLocation, nullptr);
    GameEngine::Scene::addObject("Raiden",
        "../../GameEngineCore/assets/models/raiden-shogun-genshin-impact/raiden_shogun.fbx",
        glm::vec3(50.f, 3.f, 50.f),
        glm::vec3(0.8f, 0.8f, 0.8f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        GameEngine::ShaderType::LIGHTING_TEXTURE
    );
    GameEngine::Scene::addObject("Paimon",
        "../../GameEngineCore/assets/models/paimon/paimon.obj",
        glm::vec3(51.f, 4.f, 50.f),
        glm::vec3(0.1f, 0.1f, 0.1f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        GameEngine::ShaderType::LIGHTING_TEXTURE
    );
    GameEngine::Scene::addObject("LightCube",
        lightCubeVertices,
        indices,
        std::vector<const char*>(),
        glm::vec3(51.f, 4.f, 51.f),
        glm::vec3(0.3f, 0.3f, 0.3f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        GameEngine::ShaderType::SIMPLE
    );

    GameEngine::Scene::addCamera(
        glm::vec3(50.0f, 10.0f, 55.0f),
        glm::vec3(0.f, 0.f, 0.f),
        GameEngine::ProjectionType::Perspective
    );


    GameEngine::Scene::addLight(GameEngine::LightType::DIRECTION, glm::vec3(0.0, 0.0, 0.0), sunLightDirection);
    GameEngine::Scene::addLight(GameEngine::LightType::POINT, GameEngine::ObjectManager::getObject("LightCube")->getPosition(), glm::vec3(), "LightCube");


    GameEngine::InterfaceManager::addCombo(
        "ActiveObject",
        GameEngine::ObjectManager::getObjectNames(),
        GameEngine::ObjectManager::getObjectNames()[0],
        [](std::string name) {
            GameEngine::TransformManager::updateActiveObjectName(name);
        }
    );

    GameEngine::InterfaceManager::addCombo(
        "Transform",
        std::vector<std::string>({ "Move", "Rotate", "Scale" }),
        std::string("Move"),
        [](std::string name) {

            if (name == "Move") {
                GameEngine::TransformManager::updateType(GameEngine::TransformType::Move);
            }

            if (name == "Rotate") {
                GameEngine::TransformManager::updateType(GameEngine::TransformType::Rotate);
            }

            if (name == "Scale") {
                GameEngine::TransformManager::updateType(GameEngine::TransformType::Scale);
            }
        }
    );

    GameEngine::InterfaceManager::addCombo(
        "Axis",
        std::vector<std::string>({ "X", "Y", "Z" }),
        std::string("Move"),
        [](std::string name) {

            if (name == "X") {
                GameEngine::TransformManager::updateAxis(GameEngine::TransformAxis::X);
            }

            if (name == "Y") {
                GameEngine::TransformManager::updateAxis(GameEngine::TransformAxis::Y);
            }

            if (name == "Z") {
                GameEngine::TransformManager::updateAxis(GameEngine::TransformAxis::Z);
            }
        }
    );
}


void lab2() {
    GameEngine::Scene::addCamera(
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.f, 0.f, 0.f),
        GameEngine::ProjectionType::Perspective
    );

    std::vector<GameEngine::Vertex> rectangleVertices;
    int N = 8;
    int r = 1;

    for (int i = 0; i < N; i++) {
        GLfloat x = r * cos(2 * M_PI * i / N);
        if (abs(x) < 0.0001) x = 0;
        GLfloat y = r * sin(2 * M_PI * i / N);
        if (abs(y) < 0.0001) y = 0;
        GameEngine::Vertex vertex = GameEngine::Vertex(glm::vec3(x, y, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f));
        rectangleVertices.push_back(vertex);
    }

    addFigure(rectangleVertices, std::vector<unsigned int>(), GameEngine::DrawType::TrianglesFan);


    GameEngine::InterfaceManager::addCombo(
        "Task 1",
        std::vector<std::string>({ "1", "2", "3" }),
        std::string("Task 1"),
        [rectangleVertices](std::string name) {
            GameEngine::Renderer::enableCullFace(false);
            if (name == "1") {

                GameEngine::Scene::removeObject("NRectangle");

                addFigure(rectangleVertices, std::vector<unsigned int>(), GameEngine::DrawType::TrianglesFan);

            }
            if (name == "2") {
                GameEngine::Scene::removeObject("NRectangle");
                addFigure(rectangleVertices, std::vector<unsigned int>(), GameEngine::DrawType::Lines);
            }
            if (name == "3") {
                GameEngine::Scene::removeObject("NRectangle");
                addFigure(rectangleVertices, std::vector<unsigned int>(), GameEngine::DrawType::Points);
            }
        }
    );


    GameEngine::InterfaceManager::addCombo(
        "Task 2",
        std::vector<std::string>({ "Triangle", "Trapezium", "Quadrangle", "Parallelogram", "Rectangle", "Deltoid", "Rhombus" }),
        std::string("Task"),
        [rectangleVertices](std::string name) {
            GameEngine::Renderer::enableCullFace(false);
            GameEngine::Scene::removeObject("NRectangle");
            std::map<std::string, std::vector<unsigned int>> indices = {
               std::pair<std::string, std::vector<unsigned int>>("Triangle", {3, 5, 7}),
               std::pair<std::string, std::vector<unsigned int>>("Trapezium", { 0, 1, 4, 1, 3, 4}),
               std::pair<std::string, std::vector<unsigned int>>("Quadrangle", {1, 3, 5, 1, 5, 7}),
               std::pair<std::string, std::vector<unsigned int>>("Parallelogram", {3, 4, 7, 3, 7, 0}),
               std::pair<std::string, std::vector<unsigned int>>("Rectangle", {1, 3, 5, 1, 5, 7}),
               std::pair<std::string, std::vector<unsigned int>>("Deltoid", {1, 2, 6, 2, 3, 6}),
               std::pair<std::string, std::vector<unsigned int>>("Rhombus", {0, 2, 4, 0, 4, 6}),
            };

            addFigure(rectangleVertices, indices[name]);
        }
    );

    GameEngine::InterfaceManager::addCombo(
        "Task 3",
        std::vector<std::string>({ "1", "2", "3" }),
        std::string("Task"),
        [](std::string name) {
            if (name == "1") {
                GameEngine::Renderer::enableCullFace(true);
                GameEngine::Renderer::setPolygonMode(GameEngine::CullType::Point, GameEngine::CullType::Fill);
            }

            if (name == "2") {
                GameEngine::Renderer::enableCullFace(true);
                GameEngine::Renderer::setPolygonMode(GameEngine::CullType::Fill, GameEngine::CullType::Line);
            }

            if (name == "3") {
                GameEngine::Renderer::enableCullFace(true);
                GameEngine::Renderer::setPolygonMode(GameEngine::CullType::Line, GameEngine::CullType::Line);
            }

        }
    );


    GameEngine::InterfaceManager::addCombo(
        "Task 4",
        std::vector<std::string>({ "Task 4" }),
        std::string("Task"),
        [](std::string name) {
            GameEngine::Renderer::enableCullFace(false);
            GameEngine::Scene::removeObject("NRectangle");
            std::default_random_engine generator;
            std::uniform_real_distribution<GLfloat> pointRandom(-1.f, 1.f);
            std::uniform_real_distribution<GLfloat> colorRandom(0.f, 1.f);
            const int N = 300;
            std::vector<GameEngine::Vertex> vertices;
            for (int i = 0; i < N; i++) {
                GameEngine::Vertex vertex = GameEngine::Vertex(
                    glm::vec3(pointRandom(generator), pointRandom(generator), 0.0f),
                    glm::vec3(0.0f, 0.0f, 0.0f),
                    glm::vec2(0.0f, 0.0f),
                    glm::vec3(colorRandom(generator), colorRandom(generator), colorRandom(generator))
                );
                vertices.push_back(vertex);
            }


            addFigure(vertices, std::vector<unsigned int>(), GameEngine::DrawType::Points);
        }
    );
}


void lab3() {
    GameEngine::Renderer::enableDepth(true);

    GameEngine::Scene::addCamera(
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.f, 0.f, 0.f),
        GameEngine::ProjectionType::Perspective
    );


    std::map<std::string, std::vector<unsigned int>> indices = {
               std::pair<std::string, std::vector<unsigned int>>("Triangle", {3, 5, 7}),
               std::pair<std::string, std::vector<unsigned int>>("Trapezium", { 0, 1, 4, 1, 3, 4}),
               std::pair<std::string, std::vector<unsigned int>>("Quadrangle", {1, 3, 5, 1, 5, 7}),
               std::pair<std::string, std::vector<unsigned int>>("Parallelogram", {3, 4, 7, 3, 7, 0}),
               std::pair<std::string, std::vector<unsigned int>>("Rectangle", {1, 3, 5, 1, 5, 7}),
               std::pair<std::string, std::vector<unsigned int>>("Deltoid", {1, 2, 6, 2, 3, 6}),
               std::pair<std::string, std::vector<unsigned int>>("Rhombus", {0, 2, 4, 0, 4, 6}),
    };

    std::vector<GameEngine::Vertex> rectangleVertices;
    int N = 8;
    int r = 1;

    for (int i = 0; i < N; i++) {
        GLfloat x = r * cos(2 * M_PI * i / N);
        if (abs(x) < 0.0001) x = 0;
        GLfloat y = r * sin(2 * M_PI * i / N);
        if (abs(y) < 0.0001) y = 0;
        GameEngine::Vertex vertex = GameEngine::Vertex(glm::vec3(x, y, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f));
        rectangleVertices.push_back(vertex);
    }

    addFigure("Figure1", rectangleVertices, indices["Triangle"], glm::vec3(0.0f, 0.0f, 0.0f));
    addFigure("Figure2", rectangleVertices, indices["Trapezium"], glm::vec3(0.0f, 2.0f, 0.0f));
    addFigure("Figure3", rectangleVertices, indices["Quadrangle"], glm::vec3(0.0f, -2.0f, 0.0f));

    GameEngine::InterfaceManager::addCheckbox(
        "Depth",
        [](bool status) {
            GameEngine::Renderer::enableDepth(status);
        }
    );

}


class MyApp : public GameEngine::Application {
	
    void initScene() override {
       
        lab3();

    }

    void update() override {
        
        
    }

};

int main() {
	
	auto myApp = std::make_unique<MyApp>();
	int returnCode = myApp->start(1024, 768, "Shinzo wo Sasageyo!");
	


	return returnCode;
	
}