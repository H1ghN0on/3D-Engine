#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <glm/mat3x3.hpp>
#include <glm/trigonometric.hpp>
#include <glm/vec3.hpp>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

#include <chrono>
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


namespace GameEngine {




    float deltaTime = 0.0f;	// время между текущим и последним кадрами
    float lastFrame = 0.0f; // время последнего кадра
    float lastX = 400, lastY = 300;
    static bool isPerspective = true;



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
    const char* chiakiTextureLocation = "../../GameEngineCore/assets/chiaki.png";
    const char* leopardTextureLocation = "../../GameEngineCore/assets/leopard.jpg";
    const char* containerTextureLocation = "../../GameEngineCore/assets/container.png";
    const char* containerBorderTextureLocation = "../../GameEngineCore/assets/containerBorder.png";
    const char* matrixTextureLocation = "../../GameEngineCore/assets/matrix.jpg";

    //in - enter attributes
    //out - output attributes
    //uniform - global shader program variable
;

    std::unique_ptr<CameraObject> camera = nullptr;
    std::unique_ptr<Object> lightCube = nullptr;
    std::unique_ptr<Object> toyCube = nullptr;
    std::unique_ptr<ShaderProgram> p_shaderProgram = nullptr;
    std::unique_ptr<ShaderProgram> p_lightShaderProgram = nullptr;
    std::unique_ptr<ShaderProgram> p_modelShaderProgram = nullptr;

    std::unique_ptr<Texture> containerTexture = nullptr;
    std::unique_ptr<Texture> containerBorderTexture = nullptr;
    std::unique_ptr<Texture> matrixTexture = nullptr;

	Window::Window(const unsigned int width, const unsigned int height, std::string title):
        m_data({width, height, std::move(title)})
    {
		int resultCode = init();

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui_ImplOpenGL3_Init();
        ImGui_ImplGlfw_InitForOpenGL(m_pWindow, true);

	}
	Window::~Window() {
		shutdown();
	}

    void handleKeyPress(GLFWwindow* window) {

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera->translate(CameraObject::Direction::Forward, deltaTime);
 
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera->translate(CameraObject::Direction::Back, deltaTime);

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera->translate(CameraObject::Direction::Left, deltaTime);

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera->translate(CameraObject::Direction::Right, deltaTime);

        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) camera->translate(CameraObject::Direction::Up, deltaTime);

        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) camera->translate(CameraObject::Direction::Down, deltaTime);
    }

    Model* ourModel = nullptr;

    GLfloat lightCubeVertices[24] = {
            -1.0, -1.0,  -1.0,    
            1.0, -1.0,  -1.0,       
            1.0,  1.0,  -1.0,      
            -1.0,  1.0,  -1.0,     
            -1.0, -1.0, 1.0,   
            1.0, -1.0, 1.0,      
            1.0,  1.0, 1.0,         
            -1.0,  1.0, 1.0,       
    };
    
    GLfloat cubeVertices[288] = {
       -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };

    GLuint indices[36] = {
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

        LOG_INFO("Creating window '{0}', {1}x{2}", m_data.title, m_data.width, m_data.height);

        glfwSetErrorCallback([](int code, const char* description) {
                LOG_CRITICAL("GLFW error: {0}", description);
            }
        );

        if (!glfwInit()) {
            LOG_CRITICAL("Failed to initialize GLFW");
            return -1;
        }

        m_pWindow = glfwCreateWindow(m_data.width, m_data.height, m_data.title.c_str(), nullptr, nullptr);
        if (!m_pWindow)
        {
            LOG_CRITICAL("Failed to initialize window");
            return -2;
        }

        glfwSetFramebufferSizeCallback(m_pWindow,
            [](GLFWwindow* pWindow, int width, int height) {
                Renderer::setViewport(width, height);
            }
        );

        glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        //link window properties to personal WindowData structure
        glfwSetWindowUserPointer(m_pWindow, &m_data);
        
        if (!Renderer::init(m_pWindow)) {
            LOG_CRITICAL("Failed to initialize OpenGL renderer");
            return -3;
        };

        //trigger window resize
        glfwSetWindowSizeCallback(m_pWindow,
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

        glfwSetWindowCloseCallback(m_pWindow,
            [](GLFWwindow* pWindow) {
                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
                WindowCloseEvent event;

                data.eventCallbackFn(event);
            }
        );

        glfwSetCursorPosCallback(m_pWindow,
            [](GLFWwindow* pWindow, double x, double y) {

                camera->rotate(x, y);

                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
                data.mouseX = x;
                data.mouseY = y;

                MouseMovedEvent event(x, y);

                data.eventCallbackFn(event);
            }
        );
       


        camera = std::make_unique<CameraObject>(
                glm::vec3(0.f, 0.f, -3.0f),
                glm::vec3(0.f, 0.f, 0.f),
                Camera::ProjectionType::Perspective
        );

        BufferLayout oneElement{
            ShaderDataType::Float3,
        };

        BufferLayout twoElement{
            ShaderDataType::Float3,
            ShaderDataType::Float3,
        };

        BufferLayout threeElement{
            ShaderDataType::Float3,
            ShaderDataType::Float3,
            ShaderDataType::Float2,
        };

        containerTexture = std::make_unique<Texture>(
            containerTextureLocation,
            Texture::Type::Diffusal,
            Texture::WrappingMode::Repeat,
            Texture::MipmapFilterMode::LinearLinear
         );

        containerBorderTexture = std::make_unique<Texture>(
            containerBorderTextureLocation,
            Texture::Type::Diffusal,
            Texture::WrappingMode::Repeat,
            Texture::MipmapFilterMode::LinearLinear
            );

        matrixTexture = std::make_unique<Texture>(
            matrixTextureLocation,
            Texture::Type::Diffusal,
            Texture::WrappingMode::Edge,
            Texture::MipmapFilterMode::LinearLinear
        );


        ourModel = new Model("../../GameEngineCore/assets/models/raiden-shogun-genshin-impact/raiden_shogun.fbx");

        //Cubes
        toyCube = std::make_unique<Object>(threeElement, cubeVertices, sizeof(cubeVertices), nullptr, 36);
        lightCube = std::make_unique<Object>(oneElement, lightCubeVertices, sizeof(lightCubeVertices), indices, 36);
      

        //Create Shader Program
        p_shaderProgram = std::make_unique<ShaderProgram>("ContainerShader.vs", "ContainerShader.frag");

        if (!p_shaderProgram->isCompiled())
        {
            return false;
        }

        p_lightShaderProgram = std::make_unique<ShaderProgram>("LightingShader.vs", "LightingShader.frag");

        p_modelShaderProgram = std::make_unique<ShaderProgram>("ModelLoadingShader.vs", "ModelLoadingShader.frag");

        if (!p_lightShaderProgram->isCompiled())
        {
            return false;
        }

        Renderer::enableDepth();
    
        return 0;

	}
    void Window::shutdown() {
        if (ImGui::GetCurrentContext()) {
            ImGui::DestroyContext();
        }
        glfwDestroyWindow(m_pWindow);
        glfwTerminate();
    }

    void Window::on_update() {

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;


        handleKeyPress(m_pWindow);

        Renderer::setClearColor(
            m_backgroundColor[0],
            m_backgroundColor[1],
            m_backgroundColor[2],
            m_backgroundColor[3]
        );

        //Renderer::clear();
        Renderer::clear(BitfieldMask::All);

        p_modelShaderProgram->bind();

        glm::mat4 viewAndProjectionMatrix = camera->update();
        p_modelShaderProgram->setMatrix4("viewAndProjectionMatrix", viewAndProjectionMatrix);
        glm::mat4 model(1.f);

        p_modelShaderProgram->setMatrix4("transformMatrix", model);

        ourModel->draw(*p_modelShaderProgram);


       // // bind diffuse map
       // glActiveTexture(GL_TEXTURE0);
       // containerTexture->bind();
       // // bind specular map
       // glActiveTexture(GL_TEXTURE1);
       // containerBorderTexture->bind();
       // glActiveTexture(GL_TEXTURE2);
       // matrixTexture->bind();
       // glm::vec3 camPos = camera->getPosition();
       // glm::mat4 viewAndProjectionMatrix = camera->update();
       // //Draw camera
       // glm::vec3 lightAmbient = glm::vec3(0.2f, 0.2f, 0.2f);
       // glm::vec3 lightDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
       // glm::vec3 lightSpecular = glm::vec3(1.0f, 1.0f, 1.0f);

     
       //p_shaderProgram->setVec3("viewPos", camPos);
       //p_shaderProgram->setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
       //p_shaderProgram->setVec3("objectColor", glm::vec3(1.0f, 1.0f, 1.0f));

       //glm::vec3 material = { 2.0f, 1.f, 32.f };


       //p_shaderProgram->setInt("material.diffuse", 0);
       //p_shaderProgram->setInt("material.specular", 1);

       //p_shaderProgram->setFloat("material.shininess", 32.f);


       // std::map<char*, std::pair<ShaderProgram::PropertyTypes, std::any>> dirLight = {
       //     { "ambient", { ShaderProgram::PropertyTypes::Vec3, glm::vec3(0.05f, 0.05f, 0.05f) } },
       //     { "diffuse", { ShaderProgram::PropertyTypes::Vec3, glm::vec3(0.4f, 0.4f, 0.4f) } },
       //     { "specular", { ShaderProgram::PropertyTypes::Vec3, glm::vec3(0.5, 0.5f, 0.5f) } },
       //     { "direction", { ShaderProgram::PropertyTypes::Vec3, sunLightDirection } },
       // };

       // std::map<char*, std::pair<ShaderProgram::PropertyTypes, std::any>> spotLight = {
       //     { "ambient", { ShaderProgram::PropertyTypes::Vec3, glm::vec3(0.05f, 0.05f, 0.05f) } },
       //     { "diffuse", { ShaderProgram::PropertyTypes::Vec3, glm::vec3(0.4f, 0.4f, 0.4f) } },
       //     { "specular", { ShaderProgram::PropertyTypes::Vec3, glm::vec3(0.5, 0.5f, 0.5f) } },
       //     { "cutOff", { ShaderProgram::PropertyTypes::Float, glm::cos(glm::radians(12.5f)) } },
       //     { "outerCutOff", { ShaderProgram::PropertyTypes::Float, glm::cos(glm::radians(17.5f)) } },
       //     { "position", { ShaderProgram::PropertyTypes::Vec3, camera->getPosition() } },
       //     { "direction", { ShaderProgram::PropertyTypes::Vec3, camera->getFront() } },
       //     { "constant", { ShaderProgram::PropertyTypes::Float, 1.0f } },
       //     { "linear", { ShaderProgram::PropertyTypes::Float, 0.09f } },
       //     { "quadratic", { ShaderProgram::PropertyTypes::Float, 0.032f } },
       // };

       // std::map<char*, std::pair<ShaderProgram::PropertyTypes, std::any>> pointLight = {
       //     { "ambient", { ShaderProgram::PropertyTypes::Vec3, glm::vec3(0.0f, 0.0f, 0.0f) } },
       //     { "diffuse", { ShaderProgram::PropertyTypes::Vec3, glm::vec3(1.0f, 1.0f, 1.0f) } },
       //     { "specular", { ShaderProgram::PropertyTypes::Vec3, glm::vec3(1.0f, 1.0f, 1.0f) } },
       //     { "constant", { ShaderProgram::PropertyTypes::Float, 1.0f } },
       //     { "linear", { ShaderProgram::PropertyTypes::Float, 0.09f } },
       //     { "quadratic", { ShaderProgram::PropertyTypes::Float, 0.032f } },
       // };

       // std::vector<std::map<char*, std::pair<ShaderProgram::PropertyTypes, std::any>>> pointLights;

       // for (auto& pos : lightPositions) {
       //     pointLight["position"] = { ShaderProgram::PropertyTypes::Vec3, pos };
       //     pointLights.push_back(pointLight);
       //     pointLight.erase("position");
       // }



       //p_shaderProgram->setObject("dirLight", dirLight);
       //p_shaderProgram->setObject("spotLight", spotLight);
       //p_shaderProgram->setObjects("pointLights", pointLights);

       //auto transformMatrix = toyCube->update(containerScale, containerTranslate, containerRotate);

       //Renderer::draw(*(toyCube->getVertexArray()));

       // for (unsigned int i = 0; i < 10; i++)
       // {
       //     float angle = 20.0f * i;
       //     auto transformMatrix = toyCube->update(containerScale, containerPositions[i], angle);
       //     p_shaderProgram->setMatrix4("viewAndProjectionMatrix", viewAndProjectionMatrix);
       //     p_shaderProgram->setMatrix4("transformMatrix", transformMatrix);
       //     Renderer::draw(*(toyCube->getVertexArray()));
       // }


       // //Draw light cube
       // //lightPosition.x = sin(currentFrame) * 1.5;
       // //lightPosition.z = cos(currentFrame) * 1.5;


       //p_lightShaderProgram->bind();

     


       //for (unsigned int i = 0; i < 4; i++)
       // {
       //     auto transformMatrix = lightCube->update(lightScale, lightPositions[i], lightRotate);
       //     p_lightShaderProgram->setMatrix4("viewAndProjectionMatrix", viewAndProjectionMatrix);
       //     p_lightShaderProgram->setMatrix4("transformMatrix", transformMatrix);
       //     Renderer::draw(*(toyCube->getVertexArray()));
       // }


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

        glfwSwapBuffers(m_pWindow);
        glfwPollEvents();

    }
   
}