#include <GLFW/glfw3.h>
#include <glm/mat3x3.hpp>
#include <glm/trigonometric.hpp>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

#include "GameEngineCore/Window.hpp"
#include "GameEngineCore/Log.hpp"
#include "GameEngineCore/Rendering/OpenGL/ShaderProgram.hpp"
#include "GameEngineCore/Rendering/OpenGL/VertexBuffer.hpp"
#include "GameEngineCore/Rendering/OpenGL/VertexArray.hpp"
#include "GameEngineCore/Rendering/OpenGL/IndexBuffer.hpp"
#include "GameEngineCore/Rendering/OpenGL/Camera.hpp"
#include "GameEngineCore/Rendering/OpenGL/Texture.hpp"
#include "GameEngineCore/Rendering/OpenGL/Renderer.hpp"

namespace GameEngine {
  
    static bool isPerspective = false;

    GLfloat cubeVertices[] = {
        -1.0, -1.0,  -1.0,      1.0f, 1.0f, 0.0f,      1.0f, 1.0f,
        1.0, -1.0,  -1.0,       0.0f, 1.0f, 1.0f,      0.0f, 1.0f,
        1.0,  1.0,  -1.0,       1.0f, 0.0f, 1.0f,      0.0f, 0.0f,
        -1.0,  1.0,  -1.0,      1.0f, 0.0f, 0.0f,      1.0f, 0.0f,
        -1.0, -1.0, 1.0,        1.0f, 1.0f, 0.0f,      1.0f, 1.0f,
        1.0, -1.0, 1.0,         0.0f, 1.0f, 1.0f,      0.0f, 1.0f,
        1.0,  1.0, 1.0,         1.0f, 0.0f, 1.0f,      0.0f, 0.0f,
        -1.0,  1.0, 1.0,        1.0f, 0.0f, 0.0f,      1.0f, 0.0f
    };

    GLfloat rectangleVertices[] = {
       -0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 1.0f,
       -0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 1.0f,
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f
    };

    GLfloat pointsAndColors[] = {
        0.0f, 0.5f, 0.0f,       1.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.0f,      0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 1.0f,
    };

    GLuint indices[] = {
        0, 1, 2, 3, 2, 1
    };

    GLint cubeIndices[] = {
                 0, 2, 1,
                0, 3, 2,

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

    float initPositions[6][3] = {
        { 0.f, 0.f, 0.f },
        { 2.0f,  5.0f, -15.0f },
        { -1.5f, -2.2f, -2.5f },
        { -3.8f, -2.0f, -12.3f },
        { 2.4f, -0.4f, -3.5f},
        { -1.7f,  3.0f, -7.5f}
    };

    float cameraPosition[] = { 0.f, 0.f, 1.f };
    float cameraRotation[] = {0.f, 0.f, 0.f};

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
        layout (location = 2) in vec2 texturePosition; 
        uniform mat4 transformMatrix;
        uniform mat4 viewAndProjectionMatrix;
        out vec3 color;
        out vec2 textureCoord;
        void main() {
           color = vertexColor;
           gl_Position = viewAndProjectionMatrix * transformMatrix * vec4(vertexPosition, 1.0);
           textureCoord = texturePosition;
        })";
    
    const char* fragmentShader =
        R"(#version 460
        in vec3 color;
        in vec2 textureCoord;
        out vec4 fragColor;

        uniform sampler2D ourTexture;
   
        void main() {
        fragColor = texture(ourTexture, textureCoord) * vec4(color, 1.0f);

        })";
;

    BufferLayout oneElement{
        ShaderDataType::Float3,
        ShaderDataType::Float3,
        ShaderDataType::Float2
    };


    GLuint vao;
    GLuint texture;

    std::unique_ptr<ShaderProgram> p_shaderProgram = nullptr;

    std::unique_ptr<VertexBuffer> p_cubeVBO = nullptr;


    std::unique_ptr<VertexArray> p_oneBufferVAO = nullptr;

    std::unique_ptr<IndexBuffer> p_indexBuffer = nullptr;

    Camera camera;
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

        Renderer::enableDepth();

       
        //Create Shader Program
        p_shaderProgram = std::make_unique<ShaderProgram>(vertexShader, fragmentShader);

        if (!p_shaderProgram->isCompiled())
        {
            return false;
        }

        chiakiTexture.createTexture(chiakiTextureLocation, Texture::WrappingMode::Repeat, Texture::MipmapFilterMode::LinearLinear);
        leopardTexture.createTexture(leopardTextureLocation, Texture::WrappingMode::Repeat, Texture::MipmapFilterMode::LinearLinear);
        //keeping vertexes in gpu memory
        //generating the points buffer
  
        p_cubeVBO = std::make_unique<VertexBuffer>(cubeVertices, sizeof(cubeVertices), oneElement);
        //generating vertex array object (vertex info container) and connecting to it
        p_oneBufferVAO = std::make_unique<VertexArray>();
        p_indexBuffer = std::make_unique<IndexBuffer>(cubeIndices, sizeof(cubeIndices)/sizeof(GLuint));

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
        float radiansRotate = glm::radians(rotate);

        glm::mat4 scaleMatrix(
            scale[0], 0, 0, 0,
            0, scale[1], 0, 0,
            0, 0, scale[2], 0,
            0, 0, 0, 1
        );


        if (isPerspective) {
            camera.setType(Camera::ProjectionType::Perspective);
        }
        else {
            camera.setType(Camera::ProjectionType::Orthographic);
        }

        camera.setPositionAndRotation(
            glm::vec3(cameraPosition[0], cameraPosition[1], cameraPosition[2]), 
            glm::vec3(cameraRotation[0], cameraRotation[1], cameraRotation[2])
        );
        glm::mat4 viewAndProjectionMatrix = camera.getProjectionMatrix() * camera.getViewMatrix();
        p_shaderProgram->setMatrix4("viewAndProjectionMatrix", viewAndProjectionMatrix);

        p_oneBufferVAO->bind();
   
        //draw triangle
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        for (int i = 0; i < 6; i++) {
            glm::mat4 translateMatrix(
                1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                initPositions[i][0], initPositions[i][1], initPositions[i][2], 1
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
        }
        
        //draw rectangle
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
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
        ImGui::TextColored(ImVec4(1, 0, 1, 1), "Transform Camera");
        ImGui::SliderFloat3("Camera Translate", cameraPosition, -5.f, 5.f);
        ImGui::SliderFloat3("Camera Rotate", cameraRotation, 0.f, 360.f);
        ImGui::Checkbox("Perspective", &isPerspective);
        ImGui::End();


        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(m_pWindow);
        glfwPollEvents();

    }
   
}