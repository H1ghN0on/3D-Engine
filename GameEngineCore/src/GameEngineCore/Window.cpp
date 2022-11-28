#include <glad/glad.h>
#include <glm/gtx/string_cast.hpp>
#include <GLFW/glfw3.h>
#include <glm/mat3x3.hpp>
#include <glm/trigonometric.hpp>
#include <glm/vec3.hpp>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

#include <chrono>
#include <GameEngineCore/Scene.hpp>
#include <tuple>
#include <math.h>
#include "GameEngineCore/Window.hpp"
#include "GameEngineCore/Log.hpp"
#include "GameEngineCore/Rendering/OpenGL/ShaderProgram.hpp"
#include "GameEngineCore/Rendering/OpenGL/VertexBuffer.hpp"
#include "GameEngineCore/Rendering/OpenGL/VertexArray.hpp"
#include "GameEngineCore/Rendering/OpenGL/IndexBuffer.hpp"
#include "GameEngineCore/Rendering/OpenGL/CameraObject.hpp"
#include "GameEngineCore/Rendering/OpenGL/Object.hpp"
#include "GameEngineCore/Rendering/OpenGL/Texture.hpp"
#include "GameEngineCore/Rendering/OpenGL/Renderer.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <GameEngineCore/Rendering/OpenGL/Model.hpp>
#include <GameEngineCore/Rendering/OpenGL/Terrain.hpp>
#include <GameEngineCore/Rendering/OpenGL/ObjectManager.hpp>
#include <GameEngineCore/Rendering/OpenGL/ShaderManager.hpp>

namespace GameEngine {


    float deltaTime = 0.0f;	// время между текущим и последним кадрами
    float lastFrame = 0.0f; // время последнего кадра
    float lastX = 400, lastY = 300;
    static bool isPerspective = true;

    glm::mat4 viewprojection;

    //properties of transform matrix
    glm::vec3 containerScale = { 1.0f, 1.0f, 1.0f };
    float containerRotate = 0.f;
    glm::vec3 containerTranslate = { 0.f, 0.f, 0.f };
    glm::vec3 containerPositions[] = {
         glm::vec3(0.0f,  0.0f,  0.0f),
         glm::vec3(2.0f,  5.0f, -15.0f),
         glm::vec3(-1.5f, -2.2f, -2.5f),
         glm::vec3(-3.8f, -2.0f, -12.3f),
         glm::vec3(2.4f, -0.4f, -3.5f),
         glm::vec3(-1.7f,  3.0f, -7.5f),
         glm::vec3(1.3f, -2.0f, -2.5f),
         glm::vec3(1.5f,  2.0f, -2.5f),
         glm::vec3(1.5f,  0.2f, -1.5f),
         glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    glm::vec3 lightPositions[] = {
        glm::vec3(0.7f,  0.2f,  2.0f),
        glm::vec3(2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3(0.0f,  0.0f, -3.0f)
    };

    glm::vec3 lightScale = { 0.2f, 0.2f, 0.2f };
    float lightRotate = 0.f;
    glm::vec3 lightPosition = { 1.2f, 1.0f, 2.0f };

    glm::vec3 sunLightDirection = { -0.2f, -1.0f, -0.3f };


    int width = 5, height;
    
    bool cursorEnabled = true;

    const char* chiakiTextureLocation = "../../GameEngineCore/assets/chiaki.png";
    const char* leopardTextureLocation = "../../GameEngineCore/assets/leopard.jpg";
    const char* containerTextureLocation = "../../GameEngineCore/assets/container.png";
    const char* containerBorderTextureLocation = "../../GameEngineCore/assets/containerBorder.png";
    const char* matrixTextureLocation = "../../GameEngineCore/assets/matrix.jpg";
    const char* terrainTextureLocation = "../../GameEngineCore/assets/ground.jpg";
    const char* heightMapLocation = "../../GameEngineCore/assets/heightMap.png";

    //in - enter attributes
    //out - output attributes
    //uniform - global shader program variable
;

    CameraObject* camera = nullptr;
    std::unique_ptr<Object> toyCube = nullptr;

    Terrain* terrain = nullptr;
    Terrain* terrain2 = nullptr;
    std::unique_ptr<Texture> containerTexture = nullptr;
    std::unique_ptr<Texture> containerBorderTexture = nullptr;
    std::unique_ptr<Texture> matrixTexture = nullptr;
    std::unique_ptr<Texture> terrainTexture = nullptr;

	Window::Window(const unsigned int width, const unsigned int height, std::string title):
        data({width, height, std::move(title)})
    {
		int resultCode = init();

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui_ImplOpenGL3_Init();
        ImGui_ImplGlfw_InitForOpenGL(window, true);

	}
	Window::~Window() {
		shutdown();
	}

    void handleKeyPress(GLFWwindow* window) {

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) ObjectManager::getCamera()->translate(CameraObject::Direction::Forward, deltaTime);
 
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) ObjectManager::getCamera()->translate(CameraObject::Direction::Back, deltaTime);

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) ObjectManager::getCamera()->translate(CameraObject::Direction::Left, deltaTime);

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) ObjectManager::getCamera()->translate(CameraObject::Direction::Right, deltaTime);

        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) ObjectManager::getCamera()->translate(CameraObject::Direction::Up, deltaTime);

        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) ObjectManager::getCamera()->translate(CameraObject::Direction::Down, deltaTime);
            
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) {
            cursorEnabled = false;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        };

        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
            cursorEnabled = true;
            
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        };
    }

    Model* ourModel = nullptr;


    std::vector<Vertex> lightCubeVertices = {
            Vertex(glm::vec3(-1.0, -1.0, -1.0), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
            Vertex(glm::vec3(1.0, -1.0, -1.0), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
            Vertex(glm::vec3(1.0, 1.0, -1.0), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
            Vertex(glm::vec3(-1.0,  1.0, -1.0), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
            Vertex(glm::vec3(-1.0, -1.0, 1.0), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
            Vertex(glm::vec3(1.0, -1.0, 1.0), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
            Vertex(glm::vec3(1.0,  1.0, 1.0), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
            Vertex(glm::vec3(-1.0,  1.0, 1.0), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
    };


    std::vector<Vertex> cubeVertices = {
        Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
        Vertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
        Vertex(glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
        Vertex(glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
        Vertex(glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
        Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)),

        Vertex(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
        Vertex(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
        Vertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
        Vertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
        Vertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
        Vertex(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)),

        Vertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
        Vertex(glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
        Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
        Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
        Vertex(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
        Vertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)),

        Vertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
        Vertex(glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
        Vertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
        Vertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
        Vertex(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
        Vertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)),

        Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
        Vertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
        Vertex(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
        Vertex(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
        Vertex(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
        Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f)),

        Vertex(glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
        Vertex(glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
        Vertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
        Vertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
        Vertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
        Vertex(glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f))
    };
    
    
    std::vector<GLuint> indices = {
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

	int Window::init() {

        LOG_INFO("Creating window '{0}', {1}x{2}", data.title, data.width, data.height);

        glfwSetErrorCallback([](int code, const char* description) {
                LOG_CRITICAL("GLFW error: {0}", description);
            }
        );

        if (!glfwInit()) {
            LOG_CRITICAL("Failed to initialize GLFW");
            return -1;
        }

        window = glfwCreateWindow(data.width, data.height, data.title.c_str(), nullptr, nullptr);
        if (!window)
        {
            LOG_CRITICAL("Failed to initialize window");
            return -2;
        }

        glfwSetFramebufferSizeCallback(window,
            [](GLFWwindow* pWindow, int width, int height) {
                Renderer::setViewport(width, height);
            }
        );

        glfwSetWindowUserPointer(window, &data);
        
        if (!Renderer::init(window)) {
            LOG_CRITICAL("Failed to initialize OpenGL renderer");
            return -3;
        };


        //trigger window resize
        glfwSetWindowSizeCallback(window,
            [](GLFWwindow* pWindow, int width, int height) {
                //getting the reference of personal WindowData structure
                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
                data.width = width;
                data.height = height;
                WindowResizeEvent event(width, height);

                //Application callback time!
                data.eventCallbackFn(event);
            }
        );

        glfwSetWindowCloseCallback(window,
            [](GLFWwindow* pWindow) {
                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
                WindowCloseEvent event;

                data.eventCallbackFn(event);
            }
        );

        glfwSetCursorPosCallback(window,
            [](GLFWwindow* pWindow, double x, double y) {
                if (!cursorEnabled) {
                    ObjectManager::getCamera()->rotate(x, y);
                }
             
                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
                data.mouseX = x;
                data.mouseY = y;
                MouseMovedEvent event(x, y);
                data.eventCallbackFn(event);
            }
        );

        if (ShaderManager::init() != 0) {
            LOG_CRITICAL("Failed to initialize shaders");
            return -4;
        }

            


        
    
        Scene::addTerrain("Terrain1", 0, 0, terrainTextureLocation, heightMapLocation);
        Scene::addTerrain("Terrain2", 1, 0, terrainTextureLocation, heightMapLocation);
        Scene::addObject("Raiden", 
            "../../GameEngineCore/assets/models/raiden-shogun-genshin-impact/raiden_shogun.fbx",
            glm::vec3(50.f, 3.f, 50.f),
            glm::vec3(0.8f, 0.8f, 0.8f),
            0.f,
            ShaderType::LIGHTING_TEXTURE
        );
        Scene::addObject("Paimon", 
            "../../GameEngineCore/assets/models/paimon/paimon.obj",
            glm::vec3(51.f, 4.f, 50.f),
            glm::vec3(0.1f, 0.1f, 0.1f),
            0.f,
            ShaderType::LIGHTING_TEXTURE
        );
        Scene::addObject("LightCube", 
            lightCubeVertices, indices, std::vector<Texture>(),
            glm::vec3(51.f, 4.f, 51.f),
            glm::vec3(0.3f, 0.3f, 0.3f),
            0.f,
            ShaderType::SIMPLE
        );

        Scene::addCamera(
            glm::vec3(50.0f, 10.0f, 55.0f),
            glm::vec3(0.f, 0.f, 0.f),
            Camera::ProjectionType::Perspective
        );
      

        Scene::addLight(LightType::DIRECTION, glm::vec3(0.0, 0.0, 0.0), sunLightDirection);
        Scene::addLight(LightType::POINT, ObjectManager::getObject("LightCube")->getPosition());

        Renderer::enableDepth();
    

        return 0;

	}
    void Window::shutdown() {
        if (ImGui::GetCurrentContext()) {
            ImGui::DestroyContext();
        }
        glfwDestroyWindow(window);
        glfwTerminate();
    }


    void Window::on_update() {

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;


        handleKeyPress(window);

        Renderer::setClearColor(
            bgColor[0],
            bgColor[1],
            bgColor[2],
            bgColor[3]
        );

        //Renderer::clear();
        Renderer::clear(BitfieldMask::All);
  
        Scene::render();
       
        //GUI
        /*ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize.x = static_cast<float>(get_width());
        io.DisplaySize.y = static_cast<float>(get_height());

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();

        ImGui::NewFrame();


        ImGui::Begin("Background Color Window");
        ImGui::ColorEdit4("Background Color", m_backgroundColor);
        ImGui::TextColored(ImVec4(1, 0, 1, 1), "Transform Object");
        ImGui::SliderFloat3("Scale", glm::value_ptr(scale), 0.f, 2.f);
        ImGui::SliderFloat("Rotate", &rotate, 0.f, 360.f);
        ImGui::SliderFloat3("Translate", glm::value_ptr(translate), -5.f, 5.f);
        ImGui::Checkbox("Perspective", &isPerspective);
        ImGui::End();


        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());*/

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

}