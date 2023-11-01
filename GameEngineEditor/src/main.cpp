#include <iostream>
#include <memory>
#include <random>
#include <cmath>
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
#include <thread>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <chrono>

float r = 0;
float colorChangeSpeed = 0.2f;

const char* containerTextureLocation = "../../GameEngineCore/assets/container.png";
const char* containerBorderTextureLocation = "../../GameEngineCore/assets/containerBorder.png";
const char* terrainTextureLocation = "../../GameEngineCore/assets/ground.jpg";
const char* heightMapLocation = "../../GameEngineCore/assets/heightMap.png";
const float M_PI = 3.14159265358979324f;

glm::vec3 sunLightDirection = { -0.2f, -1.0f, -0.3f };

std::pair<std::vector<GameEngine::Vertex>, std::vector<unsigned int>>createCube() {



    std::vector<GameEngine::Vertex> vertices = {
      GameEngine::Vertex(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
      GameEngine::Vertex(glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
      GameEngine::Vertex(glm::vec3(1.0f, 1.0f, -1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
      GameEngine::Vertex(glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f)),

      GameEngine::Vertex(glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
      GameEngine::Vertex(glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
      GameEngine::Vertex(glm::vec3(1.0f,  1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
      GameEngine::Vertex(glm::vec3(1.0f, 1.0f, -1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f)),

      GameEngine::Vertex(glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
      GameEngine::Vertex(glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
      GameEngine::Vertex(glm::vec3(-1.0f,  1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
      GameEngine::Vertex(glm::vec3(1.0f,  1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),

      GameEngine::Vertex(glm::vec3(-1.0f,  -1.0f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
      GameEngine::Vertex(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
      GameEngine::Vertex(glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
      GameEngine::Vertex(glm::vec3(-1.0f,  1.0f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),

      GameEngine::Vertex(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
      GameEngine::Vertex(glm::vec3(-1.0f,  1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
      GameEngine::Vertex(glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
      GameEngine::Vertex(glm::vec3(1.0f,  1.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),

      GameEngine::Vertex(glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
      GameEngine::Vertex(glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.1f), glm::vec3(1.0f, 1.0f, 1.0f)),
      GameEngine::Vertex(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
      GameEngine::Vertex(glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
    };

    std::vector<unsigned int> indices;

    for (unsigned int i = 0; i < 36; i += 4)
    {
        indices.insert(indices.end(), { i, i + 1, i + 2 });
        indices.insert(indices.end(), { i, i + 2, i + 3 });
    }

    return std::make_pair(vertices, indices);
}
std::pair<std::vector<GameEngine::Vertex>, std::vector<unsigned int>> createCone(
    float radius,
    int num_stacks,
    float height,
    float xz,
    float y,
    glm::vec3 color
)
{
    double angle = 2 * M_PI / num_stacks;
    // Num_vertices = NUM_STACKS + 2; // Since the vertices on the bottom round of the cone are different from the surface of the bottom circle and the side, the number of fixed points should be increased.
    int num_vertices = (num_stacks + 1) * 3 + 1;
    int Num_indices = num_stacks * 2 * 3; // The triangle drawn is unchanged, so the index is unchanged.

    GameEngine::Vertex vertex3(
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f),
        glm::vec2((float)xz / 2, (float)y / 2)
    );


    float nomalize_n = height * height / (height * height + radius * radius);

    //GLuint* indices = new GLuint[num_indices];

    std::vector<GameEngine::Vertex> vertices;
    std::vector<unsigned int> indices;

    vertices.resize(num_vertices);

    vertices[0] = GameEngine::Vertex(
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.f),
        glm::vec2(xz / 2, y),
        color
    );


    for (int i = 0; i <= num_stacks; ++i) {


        GameEngine::Vertex vertex1(
            glm::vec3(cos(angle * i) * radius, -sin(angle * i) * radius, 0),
            glm::vec3(0.f, 0.f, 1.f),
            glm::vec2((float)i / (float)num_stacks * xz),
            color
        );

        GameEngine::Vertex vertex2(
            vertex1.position,
            glm::vec3(cos(angle * i), -sin(angle * i), 0.f),
            vertex1.texture,
            color
        );

        GameEngine::Vertex vertex3(
            glm::vec3(0.0f, 0.0f, height),
            glm::vec3(1 * cos(angle * i), -1 * sin(angle * i), radius * height / (height * height + radius * radius)),
            glm::vec2((float)xz / 2, (float)y / 2),
            color
        );


        vertices[i + 1] = vertex1;
        vertices[i + 2 + num_stacks] = vertex2;
        vertices[i + 1 + (num_stacks + 1) * 2] = vertex3;

        if (i != num_stacks) {
            indices.push_back(0);
            indices.push_back(i + 1);
            indices.push_back(i + 2);
            indices.push_back(i + 1 + (num_stacks + 1) * 2);
            indices.push_back(i + 3 + num_stacks);
            indices.push_back(i + 2 + num_stacks);
        }

    }

    return std::make_pair(vertices, indices);
}

std::pair<std::vector<GameEngine::Vertex>, std::vector<unsigned int>> createSphere(float radius, int sectorCount, int stackCount) {
    float x, y, z, xy;                              // vertex position
    float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal
    float s, t;                                     // vertex texCoord

    float sectorStep = 2 * M_PI / sectorCount;
    float stackStep = M_PI / stackCount;
    float sectorAngle, stackAngle;

    std::vector<GameEngine::Vertex> vertices;

    for (int i = 0; i <= stackCount; ++i)
    {
        stackAngle = M_PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
        xy = radius * cosf(stackAngle);             // r * cos(u)
        z = radius * sinf(stackAngle);              // r * sin(u)

        // add (sectorCount+1) vertices per stack
        // the first and last vertices have same position and normal, but different tex coords
        for (int j = 0; j <= sectorCount; ++j)
        {
            sectorAngle = j * sectorStep;           // starting from 0 to 2pi

            // vertex position (x, y, z)
            x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
            y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)

            // normalized vertex normal (nx, ny, nz)
            nx = x;
            ny = y;
            nz = z;

            // vertex tex coord (s, t) range between [0, 1]
            s = (float)j / sectorCount;
            t = (float)i / stackCount;

            vertices.push_back(
                GameEngine::Vertex(
                    glm::vec3(x, y, z),
                    glm::vec3(nx, ny, nz),
                    glm::vec2(s, t)
                )
            );
        }
    }
    std::vector<unsigned int> indices;
    std::vector<unsigned int> lineIndices;
    int k1, k2;
    for (int i = 0; i < stackCount; ++i) {
        k1 = i * (sectorCount + 1);     // beginning of current stack
        k2 = k1 + sectorCount + 1;      // beginning of next stack

        for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
        {
            // 2 triangles per sector excluding first and last stacks
            // k1 => k2 => k1+1
            if (i != 0)
            {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            // k1+1 => k2 => k2+1
            if (i != (stackCount - 1))
            {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }

            // store indices for lines
            // vertical lines for all stacks, k1 => k2
            lineIndices.push_back(k1);
            lineIndices.push_back(k2);
            if (i != 0)  // horizontal lines except 1st stack, k1 => k+1
            {
                lineIndices.push_back(k1);
                lineIndices.push_back(k1 + 1);
            }
        }
    }

    return std::make_pair(vertices, indices);
}
std::pair<std::vector<GameEngine::Vertex>, std::vector<unsigned int>> createPyramid()
{
    std::vector<GameEngine::Vertex> vertices =
    {
        // Bottom
        GameEngine::Vertex(glm::vec3(-0.5f, 0.0f,  0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f)),
        GameEngine::Vertex(glm::vec3(-0.5f, 0.0f,  -0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 5.0f)),
        GameEngine::Vertex(glm::vec3(0.5f, 0.0f,  -0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(5.0f, 5.0f)),
        GameEngine::Vertex(glm::vec3(0.5f, 0.0f,  0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(5.0f, 0.0f)),

        // Left
        GameEngine::Vertex(glm::vec3(-0.5f, 0.0f,  0.5f), glm::vec3(-0.8f, 0.5f, 0.0f), glm::vec2(0.0f, 0.0f)),
        GameEngine::Vertex(glm::vec3(-0.5f, 0.0f,  -0.5f), glm::vec3(-0.8f, 0.5f, 0.0f), glm::vec2(5.0f, 0.0f)),
        GameEngine::Vertex(glm::vec3(0.0f, 0.8f,  0.0f), glm::vec3(-0.8f, 0.5f, 0.0f), glm::vec2(2.5f, 5.0f)),

        // Back
        GameEngine::Vertex(glm::vec3(-0.5f, 0.0f, -0.5f), glm::vec3(0.0f, 0.5f, -0.8f), glm::vec2(5.0f, 0.f)),
        GameEngine::Vertex(glm::vec3(0.5f, 0.0f, -0.5f), glm::vec3(0.0f, 0.5f, -0.8f),glm::vec2(0.0f, 0.0f)),
        GameEngine::Vertex(glm::vec3(0.0f, 0.8f,  0.0f), glm::vec3(0.0f, 0.5f, -0.8f), glm::vec2(2.5f, 5.0f)),

        // Front
        GameEngine::Vertex(glm::vec3(0.5f, 0.0f, -0.5f), glm::vec3(0.8f, 0.5f,  0.0f), glm::vec2(0.0f, 0.0f)),
        GameEngine::Vertex(glm::vec3(0.5f, 0.0f, 0.5f), glm::vec3(0.8f, 0.5f,  0.0f), glm::vec2(5.0f, 0.0f)),
        GameEngine::Vertex(glm::vec3(0.0f, 0.8f,  0.0f), glm::vec3(0.8f, 0.5f,  0.0f), glm::vec2(2.5f, 5.0f)),

        // Front
        GameEngine::Vertex(glm::vec3(0.5f, 0.0f, 0.5f), glm::vec3(0.0f, 0.5f,  0.8f), glm::vec2(5.0f, 0.0f)),
        GameEngine::Vertex(glm::vec3(-0.5f, 0.0f, 0.5f), glm::vec3(0.0f, 0.5f,  0.8f), glm::vec2(0.0f, 0.0f)),
        GameEngine::Vertex(glm::vec3(0.0f, 0.8f,  0.0f), glm::vec3(0.0f, 0.5f,  0.8f), glm::vec2(2.5f, 5.0f))
    };

    std::vector<unsigned int> indices =
    {
        0, 1, 2,
        0, 2, 3,
        4, 6, 5,
        7, 9, 8,
        10, 12, 11,
        13, 15, 14
    };

    return std::make_pair(vertices, indices);
}

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
void addFigure(std::string name, std::vector<GameEngine::Vertex> _vertices, std::vector<unsigned int> _indices, glm::vec3 position, glm::vec3 scalation, glm::vec3 rotation, GameEngine::DrawType drawType = GameEngine::DrawType::Triangles) {
    GameEngine::Scene::addObject(
        name,
        _vertices,
        _indices,
        std::vector<const char*>(),
        position,
        scalation,
        rotation,
        GameEngine::ShaderType::SIMPLE,
        drawType
    );
}



glm::vec3 cameraInitPosition = glm::vec3(214.0f, 3.f, 25.0f);

bool isUpdating = true;


void coursework() {

    GameEngine::Renderer::enableDepth(true);
    GameEngine::Window::setBackgroundColor(glm::vec4(0.4f, 0.0f, 0.2f, 1.0f));
    auto setupRoad = []() {
        float startPositionX = 105.0f;
        float startPositionZ = 25.0f;
        float startPositionY = 0.6f;

        for (short i = 0; i < 5; i++, startPositionX += 31.f) {

            GameEngine::Scene::addObject(std::string("Road1" + std::to_string(i)),
                "../../GameEngineCore/assets/models/street/untitled.obj",
                glm::vec3(startPositionX, startPositionY, startPositionZ),
                glm::vec3(1.f, 1.f, 1.f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                GameEngine::ShaderType::LIGHTING_TEXTURE
            );

            GameEngine::Scene::addObject(std::string("Road2" + std::to_string(i)),
                "../../GameEngineCore/assets/models/street/untitled.obj",
                glm::vec3(startPositionX, startPositionY, startPositionZ + 4.3f),
                glm::vec3(1.f, 1.f, 1.f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                GameEngine::ShaderType::LIGHTING_TEXTURE
            );
        }
    };



    auto setupRoadDecorations = []() {
        float startPositionX = 60.f;
        float startPositionZ = 26.8f;
        float startPositionY = 3.f;
        float startSignPositionY = 2.f;
        float startSignPositionZ = 22.4f;

        for (short i = 0; i < 24; i++, startPositionX += 8.f) {
            GameEngine::Scene::addObject(std::string("LampPost" + std::to_string(i)),
                "../../GameEngineCore/assets/models/lampost/lampost.blend",
                glm::vec3(startPositionX, startPositionY, startPositionZ),
                glm::vec3(.6f, .5f, .8f),
                glm::vec3(0.0f, 90.0f, 0.0f),
                GameEngine::ShaderType::LIGHTING
            );

            if (i % 3 == 0) {
                GameEngine::Scene::addObject(std::string("Sign" + std::to_string(i)),
                    "../../GameEngineCore/assets/models/sign/sign.obj",
                    glm::vec3(startPositionX - 4.f, startSignPositionY, startSignPositionZ),
                    glm::vec3(.3f, .3f, .26f),
                    glm::vec3(0.0f, 0.0f, 0.0f),
                    GameEngine::ShaderType::LIGHTING
                );
                GameEngine::ObjectManager::getObject(std::string("Sign" + std::to_string(i)))->setMaterial(GameEngine::ShaderMaterial::RUBBER);
            }

            GameEngine::ObjectManager::getObject(std::string("LampPost" + std::to_string(i)))->setMaterial(GameEngine::ShaderMaterial::DEFAULT);
            GameEngine::Scene::addLight(GameEngine::LightType::POINT, GameEngine::ObjectManager::getObject("LampPost" + std::to_string(i))->getPosition() + glm::vec3(-2.0f, 1.5f, 0.0f),
                glm::vec3(0.8f, 0.0f, 0.6f), glm::vec3(0.8f, 0.0f, 0.6f), std::string("LampPost" + std::to_string(i)), 12.5f, 17.5f, glm::vec3(0.0f, 0.5f, 0.3f));
        }
    };

    auto setupGuardRails = []() {

        float startPositionX = 60.f;
        float startPositionY = 2.2f;
        float startMiddlePositionZ = 26.55f;
        float startEdgePositionZ = 22.5f;


        for (short i = 0; i < 50; i++, startPositionX += 4.79f) {
            GameEngine::Scene::addObject(std::string("EdgeGuardRail" + std::to_string(i)),
                "../../GameEngineCore/assets/models/guardrail/guardrail.obj",
                glm::vec3(startPositionX, startPositionY, startEdgePositionZ),
                glm::vec3(1.f, 1.f, 1.f),
                glm::vec3(0.0f, 90.0f, 0.0f),
                GameEngine::ShaderType::LIGHTING
            );

            GameEngine::Scene::addObject(std::string("MiddleGuardRail" + std::to_string(i)),
                "../../GameEngineCore/assets/models/guardrail/guardrail.obj",
                glm::vec3(startPositionX, startPositionY, startMiddlePositionZ),
                glm::vec3(1.f, 1.f, 1.f),
                glm::vec3(0.0f, -90.0f, 0.0f),
                GameEngine::ShaderType::LIGHTING
            );
            GameEngine::ObjectManager::getObject(std::string("EdgeGuardRail" + std::to_string(i)))->setMaterial(GameEngine::ShaderMaterial::DEFAULT);
            GameEngine::ObjectManager::getObject(std::string("MiddleGuardRail" + std::to_string(i)))->setMaterial(GameEngine::ShaderMaterial::DEFAULT);
        }
    };

    auto setupBuildings = []() {

        float startPositionX = 60.f;
        float startPositionY = .3f;
        float startPositionZ = 22.0f;
        float startAnotherPositionZ = 34.0f;
        for (short i = 0; i < 7; i++, startPositionX += 24.f) {
            GameEngine::Scene::addObject(std::string("Building" + std::to_string(i)),
                "../../GameEngineCore/assets/models/building2/Apartment building/Apartment building.fbx",
                glm::vec3(startPositionX, startPositionY, startPositionZ),
                glm::vec3(.01f, .005f, .01f),
                glm::vec3(90.0f, 0.0f, 0.0f),
                GameEngine::ShaderType::LIGHTING
            );
            GameEngine::Scene::addObject(std::string("Buildings" + std::to_string(i)),
                "../../GameEngineCore/assets/models/buildings/model.dae",
                glm::vec3(startPositionX + 16.f, startPositionY + 3.7f, startPositionZ - 5.f),
                glm::vec3(.001f, .001f, .001f),
                glm::vec3(0, -90.0f, 0),
                GameEngine::ShaderType::LIGHTING
            );
            GameEngine::ObjectManager::getObject(std::string("Building" + std::to_string(i)))->setMaterial(GameEngine::ShaderMaterial::RUBBER);
            GameEngine::ObjectManager::getObject(std::string("Buildings" + std::to_string(i)))->setMaterial(GameEngine::ShaderMaterial::RUBBER);

        }



        for (short i = 0, startPositionX = 60.0f; i < 7; i++, startPositionX += 24.f) {
            GameEngine::Scene::addObject(std::string("BuildingsRev" + std::to_string(i)),
                "../../GameEngineCore/assets/models/buildings/model.dae",
                glm::vec3(startPositionX, startPositionY + 3.7f, startAnotherPositionZ + 2.f),
                glm::vec3(.001f, .001f, .001f),
                glm::vec3(0, 90.0f, 0),
                GameEngine::ShaderType::LIGHTING
            );


            GameEngine::Scene::addObject(std::string("BuildingRev" + std::to_string(i)),
                "../../GameEngineCore/assets/models/building2/Apartment building/Apartment building.fbx",
                glm::vec3(startPositionX + 16.f, startPositionY, startAnotherPositionZ),
                glm::vec3(.01f, .005f, .01f),
                glm::vec3(90.0f, 180.0f, 0.0f),
                GameEngine::ShaderType::LIGHTING
            );
            GameEngine::ObjectManager::getObject(std::string("BuildingsRev" + std::to_string(i)))->setMaterial(GameEngine::ShaderMaterial::RUBBER);
            GameEngine::ObjectManager::getObject(std::string("BuildingRev" + std::to_string(i)))->setMaterial(GameEngine::ShaderMaterial::RUBBER);
        }

    };

    auto setupCars = []() {
        float startPositionX = 185.f;
        float startPositionY = 2.15f;
        float startPositionZ = 23.3f;
        for (short i = 0; i < 3; i++, startPositionX += 10.f) {
            GameEngine::Scene::addObject(std::string("Car" + std::to_string(i)),
                "../../GameEngineCore/assets/models/car2/Chevrolet_Camaro_SS_Low.obj",
                glm::vec3(startPositionX, startPositionY, startPositionZ),
                glm::vec3(.3f, .3f, .3f),
                glm::vec3(0.0f, 90.0f, 0.0f),
                GameEngine::ShaderType::LIGHTING
            );

            GameEngine::Scene::addObject(std::string("CarMore" + std::to_string(i)),
                "../../GameEngineCore/assets/models/car2/Chevrolet_Camaro_SS_Low.obj",
                glm::vec3(startPositionX + 5.f, startPositionY, startPositionZ + 2.2f),
                glm::vec3(.3f, .3f, .3f),
                glm::vec3(0.0f, 90.0f, 0.0f),
                GameEngine::ShaderType::LIGHTING
            );
            GameEngine::ObjectManager::getObject(std::string("Car" + std::to_string(i)))->setMaterial(GameEngine::ShaderMaterial::PEARL);
            GameEngine::ObjectManager::getObject(std::string("CarMore" + std::to_string(i)))->setMaterial(GameEngine::ShaderMaterial::PEARL);

        }

        startPositionX = 110.f;
        startPositionZ = 27.7f;

        for (short i = 0; i < 13; i++, startPositionX += 12.f) {
            GameEngine::Scene::addObject(std::string("CarToNone" + std::to_string(i)),
                "../../GameEngineCore/assets/models/car2/Chevrolet_Camaro_SS_Low.obj",
                glm::vec3(startPositionX, startPositionY, startPositionZ),
                glm::vec3(.3f, .3f, .3f),
                glm::vec3(0.0f, -90.0f, 0.0f),
                GameEngine::ShaderType::LIGHTING
            );

            GameEngine::Scene::addObject(std::string("CarToNoneMore" + std::to_string(i)),
                "../../GameEngineCore/assets/models/car2/Chevrolet_Camaro_SS_Low.obj",
                glm::vec3(startPositionX + 6.f, startPositionY, startPositionZ + 2.2f),
                glm::vec3(.3f, .3f, .3f),
                glm::vec3(0.0f, -90.0f, 0.0f),
                GameEngine::ShaderType::LIGHTING
            );
            GameEngine::ObjectManager::getObject(std::string("CarToNone" + std::to_string(i)))->setMaterial(GameEngine::ShaderMaterial::PEARL);
            GameEngine::ObjectManager::getObject(std::string("CarToNoneMore" + std::to_string(i)))->setMaterial(GameEngine::ShaderMaterial::PEARL);

        }

    };


    setupRoad();
    setupRoadDecorations();
    setupGuardRails();
    setupBuildings();
    setupCars();

    auto sphere = createSphere(12.f, 30, 30);


    GameEngine::Scene::addObject(
        "Sun",
        sphere.first,
        sphere.second,
        std::vector<const char*>(),
        glm::vec3(65.0f, 30.0f, 26.8f),
        glm::vec3(1.f, 1.f, 1.f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        GameEngine::ShaderType::SUN,
        GameEngine::DrawType::Triangles
    );





    GameEngine::Scene::addObject("Raiden",
        "../../GameEngineCore/assets/models/raiden-shogun-genshin-impact/raiden_shogun.fbx",
        glm::vec3(124.0f, 2.0f, 27.0f),
        glm::vec3(0.8f, 0.8f, 0.8f),
        glm::vec3(0.0f, 180.0f, 0.0f),
        GameEngine::ShaderType::LIGHTING_TEXTURE
    );



    GameEngine::Scene::addTerrain("Terrain1", 0, 0, terrainTextureLocation, nullptr);
    GameEngine::Scene::addTerrain("Terrain2", 1, 0, terrainTextureLocation, nullptr);

    GameEngine::Scene::addCamera(
        cameraInitPosition,
        glm::vec3(0.f, 0.f, 0.f),
        GameEngine::ProjectionType::Perspective
    );

    //GameEngine::ObjectManager::getCamera()->setSpeed(10.f);

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

    GameEngine::InterfaceManager::addCheckbox(
        "Orthographic",
        [](bool status) {

            GameEngine::ObjectManager::getCamera()->setType(
                status ?
                GameEngine::ProjectionType::Orthographic
                :
                GameEngine::ProjectionType::Perspective
            );
        }
    );





    GameEngine::Window::bindKeyPress(GLFW_KEY_UP, []() {
        GameEngine::TransformManager::updateDirection(GameEngine::TransformDirection::Forward);
        GameEngine::TransformManager::transform();
        });
    GameEngine::Window::bindKeyPress(GLFW_KEY_DOWN, []() {
        GameEngine::TransformManager::updateDirection(GameEngine::TransformDirection::Backward);
        GameEngine::TransformManager::transform();
        });




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

std::pair<std::vector<GameEngine::Vertex>, std::vector<unsigned int>> createHyperboloid()
{


    std::vector<GameEngine::Vertex> vertices;
    std::vector<unsigned int> indices;
    float p = 50.f; // Number of grid columns.
    float q = 50.f; // Number of grid rows


    // Fuctions to map the grid vertex (u_i,v_j) to the mesh vertex (f(u_i,v_j), g(u_i,v_j), h(u_i,v_j)) on the patch.
    auto f = [p, q](int i, int j) {
        return (cos((-1.f + 2.0f * (float)i / p) * M_PI) / cos((-0.4f + 0.8f * (float)j / q) * M_PI));
    };

    auto g = [p, q](int i, int j)
    {
        return (sin((-1.f + 2.0 * (float)i / p) * M_PI) / cos((-0.4 + 0.8 * (float)j / q) * M_PI));
    };

    auto h = [p, q](int i, int j)
    {
        return (tan((-0.4 + 0.8 * (float)j / q) * M_PI));
    };


    for (int j = 0; j <= q; j++)
        for (int i = 0; i <= p; i++)
        {
            vertices.push_back(GameEngine::Vertex(
                glm::vec3(f(i, j), g(i, j), h(i, j)),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec2(0.0f, 0.0f)
            ));
        }

    for (int j = 0; j < q; j++)
    {
        for (int i = 0; i <= p; i++)
        {
            indices.push_back(j * (p + 1) + i);
            indices.push_back((j + 1) * (p + 1) + i);

        }
    }
    return std::make_pair(vertices, indices);
}

void lab3() {



    GameEngine::Renderer::enableCullFace(true);
    //GameEngine::Renderer::enableDepth(true);
    GameEngine::Scene::addCamera(
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.f, 0.f, 0.f),
        GameEngine::ProjectionType::Perspective
    );


    GameEngine::InterfaceManager::addCombo(
        "Task 1",
        std::vector<std::string>({ "Euthymia" }),
        std::string("Task 1"),
        [](std::string name) {
            GameEngine::Scene::clear();
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

            addFigure("Figure1", rectangleVertices, indices["Triangle"], glm::vec3(0.0f, 2.0f, 1.5f));
            addFigure("Figure2", rectangleVertices, indices["Trapezium"], glm::vec3(0.0f, 2.0f, 3.0f));
            addFigure("Figure3", rectangleVertices, indices["Quadrangle"], glm::vec3(0.0f, 2.0f, 0.0f));
        }
    );


    GameEngine::InterfaceManager::addCombo(
        "Task 2",
        std::vector<std::string>({ "Hyperboloid" }),
        std::string("Task 2"),
        [](std::string name) {
            GameEngine::Scene::clear();


            std::vector<GameEngine::Vertex> vertices;

            auto hyper = createHyperboloid();
            GameEngine::Renderer::enableCullFace(true);
            addFigure(
                "Hyperboloid",
                hyper.first,
                hyper.second,
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.8f, 0.3f, 0.5f),
                glm::vec3(90.f, 121.f, 75.f),
                GameEngine::DrawType::TriangleStrip
            );

        }
    );

    GameEngine::InterfaceManager::addCombo(
        "Task 3",
        std::vector<std::string>({ "Cube" }),
        std::string("Task 1"),
        [](std::string name) {
            GameEngine::Scene::clear();
            auto cube = createCube();
            addFigure("Cube", cube.first, cube.second, glm::vec3(0.0f, 0.0f, 0.0f));
        }
    );

    GameEngine::InterfaceManager::addCombo(
        "Task 4",
        std::vector<std::string>({ "Tree" }),
        std::string("Task 4"),
        [](std::string name) {
            GameEngine::Scene::clear();
            auto greenCone = createCone(1, 50, 2, 3, 3, glm::vec3(0.0f, 1.0f, 0.0f));
            auto brownCone = createCone(1, 50, 2, 3, 3, glm::vec3(0.6f, 0.3f, 0.0f));
            auto whiteCone = createCone(1, 50, 2, 3, 3, glm::vec3(1.0f, 1.0f, 1.0f));

            addFigure(
                "Tree4",
                brownCone.first,
                brownCone.second,
                glm::vec3(0.0f, -3.0f, 0.0f),
                glm::vec3(0.3f, 0.3f, 1.8f),
                glm::vec3(90.0f, 0.f, 0.f)
            );

            addFigure(
                "Tree1",
                whiteCone.first,
                whiteCone.second,
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.5f, 0.5f, 0.4f),
                glm::vec3(90.0f, 0.f, 0.f)
            );

            addFigure(
                "Tree2",
                greenCone.first,
                greenCone.second,
                glm::vec3(0.0f, -1.0f, 0.0f),
                glm::vec3(0.6f, 0.6f, 0.4f),
                glm::vec3(90.0f, 0.f, 0.f)
            );

            addFigure(
                "Tree3",
                greenCone.first,
                greenCone.second,
                glm::vec3(0.0f, -2.0f, 0.0f),
                glm::vec3(0.7f, 0.7f, 0.4f),
                glm::vec3(90.0f, 0.f, 0.f)
            );
        }
    );



    GameEngine::InterfaceManager::addCheckbox(
        "Depth",
        [](bool status) {
            std::cout << status << std::endl;
            GameEngine::Renderer::enableDepth(status);
        }
    );

    GameEngine::InterfaceManager::addCheckbox(
        "Perspective",
        [](bool status) {
            if (status) {
                GameEngine::ObjectManager::getCamera()->setType(GameEngine::ProjectionType::Perspective);
            }
            else {
                GameEngine::ObjectManager::getCamera()->setType(GameEngine::ProjectionType::Orthographic);
            }

        }
    );



    GameEngine::InterfaceManager::addCheckbox(
        "Grid",
        [](bool status) {

            std::vector<GameEngine::Vertex> vertices;

            if (status) {
                GameEngine::Renderer::setPolygonMode(GameEngine::CullType::Line, GameEngine::CullType::Line);
            }
            else {
                GameEngine::Renderer::setPolygonMode(GameEngine::CullType::Fill, GameEngine::CullType::Fill);
            }
        }
    );

}

void lab3Update() {
    if (GameEngine::ObjectManager::getObject("Hyperboloid")) {
        r += colorChangeSpeed * GameEngine::Window::getDeltaTime();
        if (r >= 1.f)
        {
            colorChangeSpeed *= -1;
            r = 0.99f;
        }
        else if (r <= 0)
        {
            colorChangeSpeed *= -1;
            r = 0.01f;
        }
        GameEngine::ObjectManager::getObject("Hyperboloid")->getModel().getMeshes()[0].updateColor(glm::vec3(r, 1.f, -r));
    }
}


glm::vec3 lightColor = { 1.0f, 0.0f, 0.0f };

void changeColorToPurple() {
    float colorChangeSpeed = 1.f;
    float r = colorChangeSpeed * GameEngine::Window::getDeltaTime();
    if (lightColor.x > 0.54f) lightColor.x -= r;
    if (lightColor.z < 1.f) lightColor.z += r;
}

void changeColorToRed() {
    float colorChangeSpeed = 1.f;
    float r = colorChangeSpeed * GameEngine::Window::getDeltaTime();
    if (lightColor.x < 1.0f) lightColor.x += r;
    if (lightColor.z > 0.f) lightColor.z -= r;
}

void moveUp() {
    GameEngine::TransformManager::updateDirection(GameEngine::TransformDirection::Forward);
    GameEngine::TransformManager::updateAxis(GameEngine::TransformAxis::Y);
    GameEngine::TransformManager::transform();
}

void moveDown() {
    GameEngine::TransformManager::updateDirection(GameEngine::TransformDirection::Backward);
    GameEngine::TransformManager::updateAxis(GameEngine::TransformAxis::Y);
    GameEngine::TransformManager::transform();
}

void moveLeft() {
    GameEngine::TransformManager::updateDirection(GameEngine::TransformDirection::Forward);
    GameEngine::TransformManager::updateAxis(GameEngine::TransformAxis::X);
    GameEngine::TransformManager::transform();
}

void moveRight() {
    GameEngine::TransformManager::updateDirection(GameEngine::TransformDirection::Backward);
    GameEngine::TransformManager::updateAxis(GameEngine::TransformAxis::X);
    GameEngine::TransformManager::transform();
}

float intensivity1 = 0.1f;
float intensivity2 = 0.1f;
glm::vec3 lightColor1 = { 1.0f, 0.0f, 1.0f };
glm::vec3 lightColor2 = { 1.0f, 1.0f, 0.0f };

void intensivity1Increase() {
    float intensivityChangeSpeed = 0.05f;
    if ((int)intensivity1 != 1) {
        intensivity1 += intensivityChangeSpeed;
    }
    else {
        intensivity1 = 0.1f;
    }
}

void intensivity2Increase() {
    float intensivityChangeSpeed = 0.05f;
    if ((int)intensivity2 != 1) {
        intensivity2 += intensivityChangeSpeed;
    }
    else {
        intensivity2 = 0.1f;
    }
}



float speed = 1.f;
float rotationAngle = 0;

glm::vec3 lightColor3 = { 1.0f, 1.0f, 1.0f };

void changeSpeed(bool dir) {
    if (dir && speed < 2.f) {
        speed += 0.5f * GameEngine::Window::getDeltaTime();
    }
    else if (!dir && speed > -2.f) {
        speed -= 0.5f * GameEngine::Window::getDeltaTime();
    }
}

void lab4() {
    GameEngine::Renderer::enableDepth(true);
    GameEngine::Renderer::enableCullFace(true);

    GameEngine::Scene::addCamera(
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.f, 0.f, 0.f),
        GameEngine::ProjectionType::Perspective
    );


    GameEngine::InterfaceManager::addCombo(
        "Task 1",
        std::vector<std::string>({ "Scary Sphere" }),
        std::string("Task 1"),
        [](std::string name) {
            GameEngine::Scene::clear();
            GameEngine::Window::clearBinders();

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

            auto sphere = createSphere(1.f, 30, 30);


            GameEngine::Scene::addObject(
                "NRectangle",
                sphere.first,
                sphere.second,
                std::vector<const char*>(),
                glm::vec3(1.0f, 1.0f, -1.0f),
                glm::vec3(1.f, 1.f, 1.f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                GameEngine::ShaderType::LIGHTING,
                GameEngine::DrawType::Triangles
            );

            GameEngine::Scene::addObject(
                "LightCube",
                lightCubeVertices,
                indices,
                std::vector<const char*>(),
                glm::vec3(0.f, 4.f, 0.f),
                glm::vec3(0.1f, 0.1f, 0.1f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                GameEngine::ShaderType::SIMPLE
            );

            GameEngine::Scene::addLight(
                GameEngine::LightType::POINT,
                GameEngine::ObjectManager::getObject("LightCube")->getPosition(),
                glm::vec3(0.54f, 0.f, 1.0f),
                glm::vec3(0.0f, 0.0, 0.0f),
                "LightCube"
            );



            GameEngine::Window::bindKeyPress(GLFW_KEY_RIGHT, changeColorToPurple);
            GameEngine::Window::bindKeyPress(GLFW_KEY_LEFT, changeColorToRed);
        }
    );

    GameEngine::InterfaceManager::addCombo(
        "Task 2",
        std::vector<std::string>({ "Scary 'N Moving" }),
        std::string("Task 2"),
        [](std::string name) {
            GameEngine::Scene::clear();
            GameEngine::Window::clearBinders();

            std::vector<GameEngine::Vertex> lightCubeVertices = {
              GameEngine::Vertex(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
              GameEngine::Vertex(glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
              GameEngine::Vertex(glm::vec3(1.0f, 1.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
              GameEngine::Vertex(glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
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

            auto sphere = createSphere(1.f, 30, 30);


            GameEngine::Scene::addObject(
                "NRectangle",
                sphere.first,
                sphere.second,
                std::vector<const char*>(),
                glm::vec3(1.0f, 1.0f, -1.0f),
                glm::vec3(1.f, 1.f, 1.f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                GameEngine::ShaderType::LIGHTING,
                GameEngine::DrawType::Triangles
            );

            GameEngine::Scene::addObject(
                "LightCube1",
                lightCubeVertices,
                indices,
                std::vector<const char*>(),
                glm::vec3(0.f, 4.f, 0.f),
                glm::vec3(0.1f, 0.1f, 0.1f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                GameEngine::ShaderType::SIMPLE
            );

            GameEngine::Scene::addLight(
                GameEngine::LightType::POINT,
                GameEngine::ObjectManager::getObject("LightCube1")->getPosition(),
                glm::vec3(0.54f, 0.f, 1.0f),
                glm::vec3(0.0f, 0.0, 0.0f),
                "LightCube1"
            );

            GameEngine::TransformManager::updateActiveObjectName("LightCube1");
            GameEngine::TransformManager::updateType(GameEngine::TransformType::Move);
            GameEngine::Window::bindKeyPress(GLFW_KEY_UP, moveUp);
            GameEngine::Window::bindKeyPress(GLFW_KEY_DOWN, moveDown);
            GameEngine::Window::bindKeyPress(GLFW_KEY_RIGHT, moveRight);
            GameEngine::Window::bindKeyPress(GLFW_KEY_LEFT, moveLeft);
        }
    );

    GameEngine::InterfaceManager::addCombo(
        "Task 3",
        std::vector<std::string>({ "Pyramad" }),
        std::string("Task 3"),
        [](std::string name) {
            GameEngine::Scene::clear();
            GameEngine::Window::clearBinders();



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

            auto pyramid = createPyramid();



            GameEngine::Scene::addObject(
                "Pyramid",
                pyramid.first,
                pyramid.second,
                std::vector<const char*>(),
                glm::vec3(1.0f, 1.0f, -1.0f),
                glm::vec3(1.f, 1.f, 1.f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                GameEngine::ShaderType::LIGHTING,
                GameEngine::DrawType::Triangles
            );



            GameEngine::Scene::addObject(
                "LightCube21",
                lightCubeVertices,
                indices,
                std::vector<const char*>(),
                glm::vec3(0.f, 4.f, 0.f),
                glm::vec3(0.1f, 0.1f, 0.1f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                GameEngine::ShaderType::SIMPLE
            );

            GameEngine::Scene::addLight(
                GameEngine::LightType::POINT,
                GameEngine::ObjectManager::getObject("LightCube21")->getPosition(),
                glm::vec3(0.54f, 0.f, 1.0f),
                glm::vec3(0.0f, 0.0, 0.0f),
                "LightCube21"
            );

            GameEngine::Scene::addObject(
                "LightCube22",
                lightCubeVertices,
                indices,
                std::vector<const char*>(),
                glm::vec3(2.f, 4.f, 0.f),
                glm::vec3(0.1f, 0.1f, 0.1f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                GameEngine::ShaderType::SIMPLE
            );

            GameEngine::Scene::addLight(
                GameEngine::LightType::POINT,
                GameEngine::ObjectManager::getObject("LightCube22")->getPosition(),
                glm::vec3(0.0f, 1.f, 1.0f),
                glm::vec3(0.0f, 0.0, 0.0f),
                "LightCube22"
            );

            GameEngine::Window::bindKeyPressedOnce(GLFW_KEY_P, intensivity1Increase);
            GameEngine::Window::bindKeyPressedOnce(GLFW_KEY_Y, intensivity2Increase);
        }
    );

    GameEngine::InterfaceManager::addCombo(
        "Task 4",
        std::vector<std::string>({ "Rolling" }),
        std::string("Task 4"),
        [](std::string name) {
            GameEngine::Scene::clear();
            GameEngine::Window::clearBinders();

            auto cube = createCube();


            GameEngine::Scene::addObject(
                "JustCube",
                cube.first,
                cube.second,
                std::vector<const char*>(),
                glm::vec3(0.f, 0.f, 0.f),
                glm::vec3(0.3f, 0.3f, 0.3f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                GameEngine::ShaderType::LIGHTING
            );


            GameEngine::Scene::addObject(
                "LightCube3",
                cube.first,
                cube.second,
                std::vector<const char*>(),
                glm::vec3(0.f, 0.f, 0.f),
                glm::vec3(0.1f, 0.1f, 0.1f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                GameEngine::ShaderType::SIMPLE
            );

            GameEngine::Scene::addLight(
                GameEngine::LightType::POINT,
                GameEngine::ObjectManager::getObject("LightCube3")->getPosition(),
                glm::vec3(0.0f, 1.0f, 0.0f),
                glm::vec3(1.0f, 1.0, 1.0f),
                "LightCube3"
            );

            GameEngine::Window::bindKeyPress(GLFW_KEY_LEFT, []() {changeSpeed(false); });
            GameEngine::Window::bindKeyPress(GLFW_KEY_RIGHT, []() {changeSpeed(true); });
        }
    );



    GameEngine::InterfaceManager::addCombo(
        "Materials",
        std::vector<std::string>({ "Pearl", "Emerald", "Gold", "Rubber", "Jade" }),
        std::string("Task 4"),
        [](std::string name) {
            if (name == "Pearl") {
                GameEngine::ObjectManager::getObject("JustCube")->setMaterial(GameEngine::ShaderMaterial::PEARL);
            }
            else if (name == "Emerald") {
                GameEngine::ObjectManager::getObject("JustCube")->setMaterial(GameEngine::ShaderMaterial::EMERALD);
            }
            else if (name == "Gold") {
                GameEngine::ObjectManager::getObject("JustCube")->setMaterial(GameEngine::ShaderMaterial::GOLD);
            }
            else if (name == "Rubber") {
                GameEngine::ObjectManager::getObject("JustCube")->setMaterial(GameEngine::ShaderMaterial::RUBBER);
            }
            else if (name == "Jade") {
                GameEngine::ObjectManager::getObject("JustCube")->setMaterial(GameEngine::ShaderMaterial::JADE);
            }
        }
    );

    GameEngine::InterfaceManager::addCombo(
        "View",
        std::vector<std::string>({ "Fill", "Grid", "Vertices" }),
        "View",
        [](std::string name) {
            if (name == "Fill") {
                GameEngine::Renderer::setPolygonMode(GameEngine::CullType::Fill, GameEngine::CullType::Fill);
            }
            else if (name == "Grid") {
                GameEngine::Renderer::setPolygonMode(GameEngine::CullType::Line, GameEngine::CullType::Line);
            }
            else if (name == "Vertices") {
                GameEngine::Renderer::setPolygonMode(GameEngine::CullType::Point, GameEngine::CullType::Point);
            }
        }
    );

}

void lab4(std::vector<const char*> textures) {

    bool activeTextures = true;
    auto emptyTextures = std::vector<const char*>();


    GameEngine::Renderer::enableDepth(true);
    GameEngine::Renderer::enableCullFace(true);

    GameEngine::Scene::addCamera(
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.f, 0.f, 0.f),
        GameEngine::ProjectionType::Perspective
    );


    GameEngine::InterfaceManager::addCombo(
        "Task 1",
        std::vector<std::string>({ "Scary Sphere" }),
        std::string("Task 1"),
        [=](std::string name) {
            GameEngine::Scene::clear();
            GameEngine::Window::clearBinders();

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

            auto sphere = createSphere(1.f, 30, 30);


            GameEngine::Scene::addObject(
                "NRectangle",
                sphere.first,
                sphere.second,
                activeTextures ? textures : emptyTextures,
                glm::vec3(1.0f, 1.0f, -1.0f),
                glm::vec3(1.f, 1.f, 1.f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                GameEngine::ShaderType::LIGHTING_TEXTURE,
                GameEngine::DrawType::Triangles
            );

            GameEngine::Scene::addObject(
                "LightCube",
                lightCubeVertices,
                indices,
                std::vector<const char*>(),
                glm::vec3(0.f, 4.f, 0.f),
                glm::vec3(0.1f, 0.1f, 0.1f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                GameEngine::ShaderType::SIMPLE
            );

            GameEngine::Scene::addLight(
                GameEngine::LightType::POINT,
                GameEngine::ObjectManager::getObject("LightCube")->getPosition(),
                glm::vec3(0.54f, 0.f, 1.0f),
                glm::vec3(0.0f, 0.0, 0.0f),
                "LightCube"
            );



            GameEngine::Window::bindKeyPress(GLFW_KEY_RIGHT, changeColorToPurple);
            GameEngine::Window::bindKeyPress(GLFW_KEY_LEFT, changeColorToRed);
        }
    );

    GameEngine::InterfaceManager::addCombo(
        "Task 2",
        std::vector<std::string>({ "Scary 'N Moving" }),
        std::string("Task 2"),
        [=](std::string name) {
            GameEngine::Scene::clear();
            GameEngine::Window::clearBinders();

            std::vector<GameEngine::Vertex> lightCubeVertices = {
              GameEngine::Vertex(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
              GameEngine::Vertex(glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
              GameEngine::Vertex(glm::vec3(1.0f, 1.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
              GameEngine::Vertex(glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
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

            auto sphere = createSphere(1.f, 30, 30);


            GameEngine::Scene::addObject(
                "NRectangle",
                sphere.first,
                sphere.second,
                activeTextures ? textures : emptyTextures,
                glm::vec3(1.0f, 1.0f, -1.0f),
                glm::vec3(1.f, 1.f, 1.f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                GameEngine::ShaderType::LIGHTING_TEXTURE,
                GameEngine::DrawType::Triangles
            );

            GameEngine::Scene::addObject(
                "LightCube1",
                lightCubeVertices,
                indices,
                std::vector<const char*>(),
                glm::vec3(0.f, 4.f, 0.f),
                glm::vec3(0.1f, 0.1f, 0.1f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                GameEngine::ShaderType::SIMPLE
            );

            GameEngine::Scene::addLight(
                GameEngine::LightType::POINT,
                GameEngine::ObjectManager::getObject("LightCube1")->getPosition(),
                glm::vec3(0.54f, 0.f, 1.0f),
                glm::vec3(0.0f, 0.0, 0.0f),
                "LightCube1"
            );

            GameEngine::TransformManager::updateActiveObjectName("LightCube1");
            GameEngine::TransformManager::updateType(GameEngine::TransformType::Move);
            GameEngine::Window::bindKeyPress(GLFW_KEY_UP, moveUp);
            GameEngine::Window::bindKeyPress(GLFW_KEY_DOWN, moveDown);
            GameEngine::Window::bindKeyPress(GLFW_KEY_RIGHT, moveRight);
            GameEngine::Window::bindKeyPress(GLFW_KEY_LEFT, moveLeft);
        }
    );

    GameEngine::InterfaceManager::addCombo(
        "Task 3",
        std::vector<std::string>({ "Pyramad" }),
        std::string("Task 3"),
        [=](std::string name) {
            GameEngine::Scene::clear();
            GameEngine::Window::clearBinders();



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

            auto pyramid = createPyramid();



            GameEngine::Scene::addObject(
                "Sphere",
                pyramid.first,
                pyramid.second,
                activeTextures ? textures : emptyTextures,
                glm::vec3(1.0f, 1.0f, -1.0f),
                glm::vec3(1.f, 1.f, 1.f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                GameEngine::ShaderType::LIGHTING_TEXTURE,
                GameEngine::DrawType::Triangles
            );

            GameEngine::Scene::addObject(
                "LightCube21",
                lightCubeVertices,
                indices,
                std::vector<const char*>(),
                glm::vec3(0.f, 4.f, 0.f),
                glm::vec3(0.1f, 0.1f, 0.1f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                GameEngine::ShaderType::SIMPLE
            );

            GameEngine::Scene::addLight(
                GameEngine::LightType::POINT,
                GameEngine::ObjectManager::getObject("LightCube21")->getPosition(),
                glm::vec3(0.54f, 0.f, 1.0f),
                glm::vec3(0.0f, 0.0, 0.0f),
                "LightCube21"
            );

            GameEngine::Scene::addObject(
                "LightCube22",
                lightCubeVertices,
                indices,
                std::vector<const char*>(),
                glm::vec3(2.f, 4.f, 0.f),
                glm::vec3(0.1f, 0.1f, 0.1f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                GameEngine::ShaderType::SIMPLE
            );

            GameEngine::Scene::addLight(
                GameEngine::LightType::POINT,
                GameEngine::ObjectManager::getObject("LightCube22")->getPosition(),
                glm::vec3(0.0f, 1.f, 1.0f),
                glm::vec3(0.0f, 0.0, 0.0f),
                "LightCube22"
            );

            GameEngine::Window::bindKeyPressedOnce(GLFW_KEY_P, intensivity1Increase);
            GameEngine::Window::bindKeyPressedOnce(GLFW_KEY_Y, intensivity2Increase);
        }
    );

    GameEngine::InterfaceManager::addCombo(
        "Task 4",
        std::vector<std::string>({ "Rolling" }),
        std::string("Task 4"),
        [=](std::string name) {
            GameEngine::Scene::clear();
            GameEngine::Window::clearBinders();

            auto cube = createCube();

            GameEngine::Scene::addObject(
                "JustCube",
                cube.first,
                cube.second,
                activeTextures ? textures : emptyTextures,
                glm::vec3(0.f, 0.f, 0.f),
                glm::vec3(0.3f, 0.3f, 0.3f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                GameEngine::ShaderType::LIGHTING_TEXTURE
            );


            GameEngine::Scene::addObject(
                "LightCube3",
                cube.first,
                cube.second,
                std::vector<const char*>(),
                glm::vec3(0.f, 0.f, 0.f),
                glm::vec3(0.1f, 0.1f, 0.1f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                GameEngine::ShaderType::SIMPLE
            );

            GameEngine::Scene::addLight(
                GameEngine::LightType::POINT,
                GameEngine::ObjectManager::getObject("LightCube3")->getPosition(),
                glm::vec3(0.0f, 1.0f, 0.0f),
                glm::vec3(1.0f, 1.0, 1.0f),
                "LightCube3"
            );

            GameEngine::Window::bindKeyPress(GLFW_KEY_LEFT, []() {changeSpeed(false); });
            GameEngine::Window::bindKeyPress(GLFW_KEY_RIGHT, []() {changeSpeed(true); });
        }
    );

    GameEngine::InterfaceManager::addCombo(
        "Materials",
        std::vector<std::string>({ "Pearl", "Emerald", "Gold", "Rubber", "Jade" }),
        std::string("Task 4"),
        [textures](std::string name) {
            if (name == "Pearl") {
                GameEngine::ObjectManager::getObject("JustCube")->setMaterial(GameEngine::ShaderMaterial::PEARL);
            }
            else if (name == "Emerald") {
                GameEngine::ObjectManager::getObject("JustCube")->setMaterial(GameEngine::ShaderMaterial::EMERALD);
            }
            else if (name == "Gold") {
                GameEngine::ObjectManager::getObject("JustCube")->setMaterial(GameEngine::ShaderMaterial::GOLD);
            }
            else if (name == "Rubber") {
                GameEngine::ObjectManager::getObject("JustCube")->setMaterial(GameEngine::ShaderMaterial::RUBBER);
            }
            else if (name == "Jade") {
                GameEngine::ObjectManager::getObject("JustCube")->setMaterial(GameEngine::ShaderMaterial::JADE);
            }
        }
    );

    GameEngine::InterfaceManager::addCombo(
        "View",
        std::vector<std::string>({ "Fill", "Grid", "Vertices" }),
        "View",
        [](std::string name) {
            if (name == "Fill") {
                GameEngine::Renderer::setPolygonMode(GameEngine::CullType::Fill, GameEngine::CullType::Fill);
            }
            else if (name == "Grid") {
                GameEngine::Renderer::setPolygonMode(GameEngine::CullType::Line, GameEngine::CullType::Line);
            }
            else if (name == "Vertices") {
                GameEngine::Renderer::setPolygonMode(GameEngine::CullType::Point, GameEngine::CullType::Point);
            }
        }
    );
    GameEngine::InterfaceManager::addCheckbox(
        "Texture", [&activeTextures](bool active) {
            activeTextures = active;

            if (active) {
                for (auto& [key, object] : GameEngine::ObjectManager::getObjects()) {
                    if (object->getShader() == GameEngine::ShaderManager::get(GameEngine::ShaderType::LIGHTING)) {
                        object->setShader(GameEngine::ShaderManager::get(GameEngine::ShaderType::LIGHTING_TEXTURE));
                    }
                }
            }
            else {
                for (auto& [key, object] : GameEngine::ObjectManager::getObjects()) {
                    if (object->getShader() == GameEngine::ShaderManager::get(GameEngine::ShaderType::LIGHTING_TEXTURE)) {
                        object->setShader(GameEngine::ShaderManager::get(GameEngine::ShaderType::LIGHTING));
                    }
                }
            }
        },
        true
            );
}

float xSin = 0, yCos = 0, zSas = 0;
void lab4Update() {

    if (GameEngine::Scene::getPointLights().size()) {

        if (GameEngine::ObjectManager::getObject("LightCube")) {
            GameEngine::Scene::updatePointLightDiffuseColor(0, lightColor);
            GameEngine::ObjectManager::getObject("LightCube")->getModel().getMeshes()[0].updateColor(lightColor);
        }

        if (GameEngine::ObjectManager::getObject("LightCube1")) {
            GameEngine::Scene::updatePointLightDiffuseColor(0, lightColor);
            GameEngine::ObjectManager::getObject("LightCube1")->getModel().getMeshes()[0].updateColor(lightColor);
        }

        if (GameEngine::ObjectManager::getObject("LightCube21")) {
            GameEngine::Scene::updatePointLightDiffuseColor(0, intensivity1 * lightColor1);
            GameEngine::ObjectManager::getObject("LightCube21")->getModel().getMeshes()[0].updateColor(intensivity1 * lightColor1);
            GameEngine::Scene::updatePointLightDiffuseColor(1, intensivity2 * lightColor2);
            GameEngine::ObjectManager::getObject("LightCube22")->getModel().getMeshes()[0].updateColor(intensivity2 * lightColor2);
        }

        if (GameEngine::ObjectManager::getObject("LightCube3")) {
            GameEngine::Scene::updatePointLightDiffuseColor(0, lightColor3);
            GameEngine::ObjectManager::getObject("LightCube3")->getModel().getMeshes()[0].updateColor(lightColor3);

            xSin = sinf(rotationAngle);
            yCos = cosf(rotationAngle);

            rotationAngle += speed * GameEngine::Window::getDeltaTime();

            GameEngine::ObjectManager::getObject("LightCube3")->setPosition(
                glm::vec3(xSin, yCos, zSas)
            );

            GameEngine::Scene::setPointLight(
                GameEngine::ObjectManager::getObject("LightCube3")->getLightIndex(),
                glm::vec3(xSin, yCos, zSas)
            );
        }
    }






}

class MyApp : public GameEngine::Application {
    float rotateSpeed = 50.f;
    float transformSpeed = 2.f;

    bool shouldBeStabilized = false;

    void setStartPositions() {
        float startPositionX = 185.f;
        float startPositionY = 2.15f;
        float startPositionZ = 23.3f;
        auto car2 = GameEngine::ObjectManager::getObject(std::string("CarMore2"));
        float activeZPos = car2->getPosition().z;
        for (short i = 0; i < 3; i++, startPositionX += 10.f) {

            GameEngine::Object* car = GameEngine::ObjectManager::getObject(std::string("Car" + std::to_string(i)));
            GameEngine::Object* car2 = GameEngine::ObjectManager::getObject(std::string("CarMore" + std::to_string(i)));
            GameEngine::Object* sun = GameEngine::ObjectManager::getObject("Sun");

            car->setPosition(glm::vec3(startPositionX, startPositionY, startPositionZ));
            car2->setPosition(glm::vec3(startPositionX + 5.f, startPositionY, startPositionZ + 2.2f));

            GameEngine::ObjectManager::getCamera()->setPosition(cameraInitPosition);
            sun->setPosition(GameEngine::ObjectManager::getCamera()->getPosition() + glm::vec3(-80.f, 30.0f, 1.8f));
        }
        glm::vec3 pos = car2->getPosition();
        car2->setPosition(glm::vec3(pos.x, pos.y, activeZPos));
    }

    void carRotate(bool side) {

        auto deltaTime = GameEngine::Window::getDeltaTime();

        float trueRotationSpeed = rotateSpeed * deltaTime;
        float trueTransformSpeed = transformSpeed * deltaTime;

        auto rotationVec = glm::vec3(0.0, trueRotationSpeed * (side ? -1 : 1), 0.0f);
        auto transformVec = glm::vec3(0.0, 0.0f, trueTransformSpeed * (side ? 1 : -1));

        GameEngine::Object* car = GameEngine::ObjectManager::getObject(std::string("CarMore2"));

        if (!side && car->getPosition().z > 23.4f) {
            auto rot = car->getRotation();
            auto pos = car->getPosition();

            car->setRotation(rot + rotationVec);
            car->setPosition(pos + transformVec);
        }
        else if (side && car->getPosition().z < 25.6f) {
            auto rot = car->getRotation();
            auto pos = car->getPosition();

            car->setRotation(rot + rotationVec);
            car->setPosition(pos + transformVec);
        }
        else {
            shouldBeStabilized = true;
        }

    }

    void carStabilize() {
        GameEngine::Object* car = GameEngine::ObjectManager::getObject(std::string("CarMore2"));

        auto rot = car->getRotation();

        if (rot.y != 90.0f) {
            glm::vec3 rotationVec;
            if (abs(rot.y - 90.0f) < 1.f) {
                car->setRotation(glm::vec3(0.0f, 90.0f, 0.0f));
            }
            else {
                auto deltaTime = GameEngine::Window::getDeltaTime();
                float trueRotationSpeed = (rotateSpeed + 25.0f) * deltaTime;
                rotationVec = glm::vec3(0.0, trueRotationSpeed * (rot.y > 90.0f ? -1.f : 1.f), 0.0f);
                car->setRotation(rot + rotationVec);
            }

        }
        else {
            shouldBeStabilized = false;
        }

    }

    void initScene() override {

        coursework();
        
        
        GameEngine::Window::bindKeyPressedOnce(GLFW_KEY_E, [&]() {
            isUpdating = !isUpdating;
            if (isUpdating) {
                setStartPositions();
            }
        });

        GameEngine::Window::bindKeyPress(GLFW_KEY_LEFT, [&]() {
            carRotate(true);
        });

        GameEngine::Window::bindKeyPress(GLFW_KEY_RIGHT, [&]() {
            carRotate(false);
        });

        GameEngine::Window::bindKeyRelease(GLFW_KEY_LEFT, [&]() {
            shouldBeStabilized = true;
        });

        GameEngine::Window::bindKeyRelease(GLFW_KEY_RIGHT, [&]() {
            shouldBeStabilized = true;
            });

    }



    void update() override {
        if (isUpdating) {
            float speed = 11.f;
            auto deltaTime = GameEngine::Window::getDeltaTime();
            float trueSpeed = speed * deltaTime;
            auto transformVec = glm::vec3(-trueSpeed, 0.0f, 0.0f);

            GameEngine::Object* car = GameEngine::ObjectManager::getObject(std::string("Car0"));

            auto pos = car->getPosition();

            if (pos.x < 137.f) {
                setStartPositions();
            }
            else {
                for (short i = 0; i < 3; i++) {
                    GameEngine::Object* car = GameEngine::ObjectManager::getObject(std::string("Car" + std::to_string(i)));
                    GameEngine::Object* car2 = GameEngine::ObjectManager::getObject(std::string("CarMore" + std::to_string(i)));
                    auto pos = car->getPosition();
                    auto pos2 = car2->getPosition();
                    car->setPosition(pos + transformVec);
                    car2->setPosition(pos2 + transformVec);
                }
                GameEngine::Object* sun = GameEngine::ObjectManager::getObject("Sun");
                GameEngine::CameraObject* cam = GameEngine::ObjectManager::getCamera();

                cam->setPosition(cam->getPosition() + transformVec);
                sun->setPosition(cam->getPosition() + glm::vec3(-80.f, 30.0f, 1.8f));
            }
            if (shouldBeStabilized) {
                carStabilize();
            }
        }


    }

};

int main() {

    auto myApp = std::make_unique<MyApp>();
    int returnCode = myApp->start(1024, 768, "Vibin'");



    return returnCode;

}