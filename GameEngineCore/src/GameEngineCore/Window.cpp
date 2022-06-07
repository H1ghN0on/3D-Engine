#include <GLFW/glfw3.h>
#include <glm/mat3x3.hpp>
#include <glm/trigonometric.hpp>
#include <glm/vec3.hpp>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

#include "GameEngineCore/Window.hpp"
#include "GameEngineCore/Log.hpp"
#include "GameEngineCore/Rendering/OpenGL/ShaderProgram.hpp"
#include "GameEngineCore/Rendering/OpenGL/VertexBuffer.hpp"
#include "GameEngineCore/Rendering/OpenGL/VertexArray.hpp"
#include "GameEngineCore/Rendering/OpenGL/IndexBuffer.hpp"
#include "GameEngineCore/Rendering/OpenGL/CameraObject.hpp"
#include "GameEngineCore/Rendering/OpenGL/Texture.hpp"
#include "GameEngineCore/Rendering/OpenGL/Renderer.hpp"

namespace GameEngine {
    float deltaTime = 0.0f;	// время между текущим и последним кадрами
    float lastFrame = 0.0f; // время последнего кадра
    float lastX = 400, lastY = 300;
    static bool isPerspective = true;

    GLfloat cubeVertices[] = {
        -1.0, -1.0,  -1.0,      1.0f, 1.0f, 0.0f,     
        1.0, -1.0,  -1.0,       0.0f, 1.0f, 1.0f,
        1.0,  1.0,  -1.0,       1.0f, 0.0f, 1.0f,     
        -1.0,  1.0,  -1.0,      1.0f, 0.0f, 0.0f,      
        -1.0, -1.0, 1.0,        1.0f, 1.0f, 0.0f,    
        1.0, -1.0, 1.0,         0.0f, 1.0f, 1.0f,     
        1.0,  1.0, 1.0,         1.0f, 0.0f, 1.0f,     
        -1.0,  1.0, 1.0,        1.0f, 0.0f, 0.0f,      
    };

    GLint cubeIndices[] = {
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

    //properties of transform matrix
    float scale[] = { 1.f, 1.f, 1.f };
    float rotate = 0.f;
    float translate[] = { 0.f, 0.f, 0.f };

    float initPositions[3] = { 0.f, 0.f, 0.f };

    int width = 5, height;
    const char* chiakiTextureLocation = "../../GameEngineCore/assets/chiaki.png";
    const char* leopardTextureLocation = "../../GameEngineCore/assets/leopard.jpg";
    
    //in - enter attributes
    //out - output attributes
    //uniform - global shader program variable
    const char* vertexShader =
        R"(#version 460
        layout(location = 0) in vec3 vertexPosition;
        layout(location = 1) in vec3 vertexColor;
        uniform mat4 transformMatrix;
        uniform mat4 viewAndProjectionMatrix;
        out vec3 color;
        void main() {
           color = vertexColor;
           gl_Position = viewAndProjectionMatrix * transformMatrix * vec4(vertexPosition, 1.0);
        })";
    
    const char* fragmentShader =
        R"(#version 460
        in vec3 color;
        out vec4 fragColor;
   
        void main() {
        fragColor = vec4(color, 1.0f);

        })";
;

    BufferLayout oneElement{
        ShaderDataType::Float3,
        ShaderDataType::Float3,
    };


    GLuint vao;
    GLuint texture;

    std::unique_ptr<ShaderProgram> p_shaderProgram = nullptr;

    std::unique_ptr<VertexBuffer> p_cubeVBO = nullptr;

    std::unique_ptr<VertexArray> p_oneBufferVAO = nullptr;

    std::unique_ptr<IndexBuffer> p_indexBuffer = nullptr;

    CameraObject camera(
        glm::vec3(0.f, 0.f, 0.f),
        glm::vec3(0.f, 0.f, 0.f),
        Camera::ProjectionType::Perspective
    );
    Texture chiakiTexture;
    Texture leopardTexture;

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

    void onKeyPressed(GLFWwindow* window) {

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera.move(CameraObject::Direction::Forward, deltaTime);
 
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera.move(CameraObject::Direction::Back, deltaTime);

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera.move(CameraObject::Direction::Left, deltaTime);

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera.move(CameraObject::Direction::Right, deltaTime);

        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) camera.move(CameraObject::Direction::Up, deltaTime);

        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) camera.move(CameraObject::Direction::Down, deltaTime);
    }

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
        
        if (!Renderer::init(m_pWindow)) {
            LOG_CRITICAL("Failed to initialize OpenGL renderer");
            return -3;
        };

        glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        //link window properties to personal WindowData structure
        glfwSetWindowUserPointer(m_pWindow, &m_data);

        //trigger window resize
        glfwSetWindowSizeCallback(m_pWindow,
            [](GLFWwindow* pWindow, int width, int height) {
                //getting the reference of personal WindowData structure
                WindowData &data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
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

                camera.rotate(x, y);

                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
                data.mouseX = x;
                data.mouseY = y;

                MouseMovedEvent event(x, y);

                data.eventCallbackFn(event);
            }
        );

  
        glfwSetFramebufferSizeCallback(m_pWindow,
            [](GLFWwindow* pWindow, int width, int height) {
                Renderer::setViewport(width, height);
            }
        );

        //glfwSetKeyCallback(m_pWindow, onKeyPressed);

        Renderer::enableDepth();

       
        //Create Shader Program
        p_shaderProgram = std::make_unique<ShaderProgram>(vertexShader, fragmentShader);

        if (!p_shaderProgram->isCompiled())
        {
            return false;
        }

        p_cubeVBO = std::make_unique<VertexBuffer>(cubeVertices, sizeof(cubeVertices), oneElement);
        //generating vertex array object (vertex info container) and connecting to it
        p_oneBufferVAO = std::make_unique<VertexArray>();
        p_indexBuffer = std::make_unique<IndexBuffer>(cubeIndices, sizeof(cubeIndices) / sizeof(GLuint));

        p_oneBufferVAO->addVertexBuffer(*p_cubeVBO);

        p_oneBufferVAO->setIndexBuffer(*p_indexBuffer);

        
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
        onKeyPressed(m_pWindow);
        Renderer::setClearColor(
            m_backgroundColor[0],
            m_backgroundColor[1],
            m_backgroundColor[2],
            m_backgroundColor[3]
        );

        //Renderer::clear();
        Renderer::clear(BitfieldMask::All);
        //connecting shaders and vao to render

        chiakiTexture.bind();

        p_shaderProgram->bind();

        //Draw camera

        camera.update();
        glm::mat4 viewAndProjectionMatrix = camera.getProjectionMatrix() * camera.getViewMatrix();
        p_shaderProgram->setMatrix4("viewAndProjectionMatrix", viewAndProjectionMatrix);

        //
        //Draw cube
        p_oneBufferVAO->bind();
        float radiansRotate = glm::radians(rotate);

        glm::mat4 scaleMatrix(
            scale[0], 0, 0, 0,
            0, scale[1], 0, 0,
            0, 0, scale[2], 0,
            0, 0, 0, 1
        );

        glm::mat4 translateMatrix(
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            translate[0], translate[1], translate[2], 1
        );
        glm::mat4 rotateMatrix(
            cos(radiansRotate), sin(radiansRotate), 0, 0,
            -sin(radiansRotate), cos(radiansRotate), 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        );

        glm::mat4 transformMatrix = translateMatrix * rotateMatrix * scaleMatrix;
        p_shaderProgram->setMatrix4("transformMatrix", transformMatrix);
        Renderer::draw(*p_oneBufferVAO);
        //
       
        
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize.x = static_cast<float>(get_width());
        io.DisplaySize.y = static_cast<float>(get_height());

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();

        ImGui::NewFrame();


        ImGui::Begin("Background Color Window");
        ImGui::ColorEdit4("Background Color", m_backgroundColor);
        ImGui::TextColored(ImVec4(1, 0, 1, 1), "Transform Object");
        ImGui::SliderFloat3("Scale", scale, 0.f, 2.f);
        ImGui::SliderFloat("Rotate", &rotate, 0.f, 360.f);
        ImGui::SliderFloat3("Translate", translate, -5.f, 5.f);
        ImGui::Checkbox("Perspective", &isPerspective);
        ImGui::End();


        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(m_pWindow);
        glfwPollEvents();

    }
   
}