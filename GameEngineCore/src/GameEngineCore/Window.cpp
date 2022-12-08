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
#include <GameEngineCore/Renderer.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <GameEngineCore/InterfaceManager.hpp>
#include <GameEngineCore/Rendering/OpenGL/Model.hpp>
#include <GameEngineCore/Rendering/OpenGL/Terrain.hpp>
#include <GameEngineCore/ObjectManager.hpp>
#include <GameEngineCore/ShaderManager.hpp>
#include <GameEngineCore/Vertex.hpp>
#include <GameEngineCore/TransformManager.hpp>

namespace GameEngine {
    GLFWwindow* Window::window = nullptr;
    std::map<int, std::function<void()>> Window::pressedKeysForListen = std::map<int, std::function<void()>>();
    std::map<int, std::function<void()>> Window::pressedOnceKeysForListen = std::map<int, std::function<void()>>();

    float deltaTime = 0.0f;	// время между текущим и последним кадрами
    float lastFrame = 0.0f; // время последнего кадра
    float lastX = 400, lastY = 300;
    static bool isPerspective = true;
    bool cursorEnabled = true;


    float Window::deltaTime = 0;
    float Window::lastFrame = 0;

	Window::Window(const unsigned int width, const unsigned int height, std::string title):
        data({width, height, std::move(title)})
    {
		int resultCode = init();

	}
	Window::~Window() {
		shutdown();
	}

    void Window::handleKeyPress() {

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) ObjectManager::getCamera()->translate(CameraDirection::Forward, Window::getDeltaTime());
 
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) ObjectManager::getCamera()->translate(CameraDirection::Back, Window::getDeltaTime());

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) ObjectManager::getCamera()->translate(CameraDirection::Left, Window::getDeltaTime());

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) ObjectManager::getCamera()->translate(CameraDirection::Right, Window::getDeltaTime());

        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) ObjectManager::getCamera()->translate(CameraDirection::Up, Window::getDeltaTime());

        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) ObjectManager::getCamera()->translate(CameraDirection::Down, Window::getDeltaTime());
            
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) {
            cursorEnabled = false;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        };

        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
            cursorEnabled = true;
            
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        };


        for (auto [key, foo] : pressedKeysForListen) {
            if (glfwGetKey(window, key) == GLFW_PRESS) foo();
        }
    }
    
    

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
                std::cout << "huy" << std::endl;
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
        glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
            {
                for (auto [keyCode, foo] : pressedOnceKeysForListen) {
                    if (keyCode == key) {
                        foo();
                    }
                }
            }
        );

        if (ShaderManager::init() != 0) {
            LOG_CRITICAL("Failed to initialize shaders");
            return -4;
        }

        InterfaceManager::init(window);

        return 0;

	}
    void Window::shutdown() {
        if (ImGui::GetCurrentContext()) {
            ImGui::DestroyContext();
        }
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void Window::updateDeltaTime() {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
    }

    void Window::bindKeyPress(int keyCode, std::function<void()> foo) {
        pressedKeysForListen[keyCode] = foo;
    }

    void Window::bindKeyPressedOnce(int keyCode, std::function<void()> foo) {

        pressedOnceKeysForListen[keyCode] = foo;
    }

    void Window::on_update() {

        updateDeltaTime();


        handleKeyPress();

        Renderer::setClearColor(
            bgColor[0],
            bgColor[1],
            bgColor[2],
            bgColor[3]
        );

        //Renderer::clear();
        Renderer::clear(BitfieldMask::All);
  
        Scene::render();
       
        InterfaceManager::render(getWidth(), getHeight());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

}