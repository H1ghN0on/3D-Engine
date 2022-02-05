#include <glad/glad.h>
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


namespace GameEngine {
   

    static bool s_GLFW_initialized = false;

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

    //properties of transform matrix
    float scale[] = { 1.f, 1.f, 1.f };
    float rotate = 0.f;
    float translate[] = { 0.f, 0.f, 0.f };


    //in - enter attributes
    //out - output attributes
    //uniform - global shader program variable
    const char* vertexShader =
        R"(#version 460
        layout(location = 0) in vec3 vertexPosition;
        layout(location = 1) in vec3 vertexColor;
        uniform mat4 transformMatrix;
        out vec3 color;
        void main() {
           color = vertexColor;
           gl_Position = transformMatrix * vec4(vertexPosition, 1.0);
        })";
    
    const char* fragmentShader =
        R"(#version 460
        in vec3 color;
        out vec4 fragColor;
   
        void main() {
        fragColor = vec4(color, 1.0);
        })";
;

    BufferLayout oneElement{
        ShaderDataType::Float3,
        ShaderDataType::Float3
    };


    GLuint vao;

    std::unique_ptr<ShaderProgram> p_shaderProgram = nullptr;

    std::unique_ptr<VertexBuffer> p_pointsAndColorsVBO = nullptr;

    std::unique_ptr<VertexArray> p_oneBufferVAO = nullptr;

    std::unique_ptr<IndexBuffer> p_indexBuffer = nullptr;


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

        if (!s_GLFW_initialized) {
            if (!glfwInit()) {
                LOG_CRITICAL("Failed to initialize GLFW");
                return -1;
            }
            s_GLFW_initialized = true;
        }

        m_pWindow = glfwCreateWindow(m_data.width, m_data.height, m_data.title.c_str(), nullptr, nullptr);
        if (!m_pWindow)
        {
            LOG_CRITICAL("Failed to initialize window");
            glfwTerminate();
            return -2;
        }

        glfwMakeContextCurrent(m_pWindow);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            LOG_CRITICAL("Failed to initialize GLAD");
            return -3;
        }

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
                glViewport(0, 0, width, height);
            }
        );
        //Create Shader Program
        p_shaderProgram = std::make_unique<ShaderProgram>(vertexShader, fragmentShader);

        if (!p_shaderProgram->isCompiled())
        {
            return false;
        }
        //keeping vertexes in gpu memory
        //generating the points buffer
  
        p_pointsAndColorsVBO = std::make_unique<VertexBuffer>(rectangleVertices, sizeof(rectangleVertices), oneElement);
        //generating vertex array object (vertex info container) and connecting to it
        p_oneBufferVAO = std::make_unique<VertexArray>();
        p_indexBuffer = std::make_unique<IndexBuffer>(indices, sizeof(indices)/sizeof(GLuint));

        p_oneBufferVAO->addVertexBuffer(*p_pointsAndColorsVBO);
        p_oneBufferVAO->setIndexBuffer(*p_indexBuffer); 


        return 0;

	}
    void Window::shutdown() {
        glfwDestroyWindow(m_pWindow);
        glfwTerminate();
    }

    void Window::on_update() {

        glClearColor(
            m_backgroundColor[0], 
            m_backgroundColor[1],
            m_backgroundColor[2],
            m_backgroundColor[3]);
        glClear(GL_COLOR_BUFFER_BIT);

        //connecting shaders and vao to render
        p_shaderProgram->bind();
        float radiansRotate = glm::radians(rotate);

        glm::mat4 scaleMatrix(
            scale[0], 0, 0, 0,
            0, scale[1], 0, 0,
            0, 0, scale[2], 0,
            0, 0, 0, 1
        );

        glm::mat4 rotateMatrix(
            cos(radiansRotate), sin(radiansRotate), 0, 0,
            -sin(radiansRotate), cos(radiansRotate), 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        );

        glm::mat4 translateMatrix(
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            translate[0], translate[1], translate[2], 1
        );

        glm::mat4 transformMatrix = translateMatrix * rotateMatrix * scaleMatrix;

        p_shaderProgram->setMatrix4("transformMatrix", transformMatrix);
        p_oneBufferVAO->bind();

        //draw triangle
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, p_oneBufferVAO->getIndicesCount(), GL_UNSIGNED_INT, nullptr);
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
        ImGui::SliderFloat3("Scale", scale, 0.f, 2.f);
        ImGui::SliderFloat("Rotate", &rotate, 0.f, 360.f);
        ImGui::SliderFloat3("Translate", translate, -1.f, 1.f);
        ImGui::End();



        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(m_pWindow);
        glfwPollEvents();

    }
   
}