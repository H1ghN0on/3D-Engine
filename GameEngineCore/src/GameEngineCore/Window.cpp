#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

#include "GameEngineCore/Window.hpp"
#include "GameEngineCore/Log.hpp"
#include "GameEngineCore/Rendering/OpenGL/ShaderProgram.hpp"
#include "GameEngineCore/Rendering/OpenGL/VertexBuffer.hpp"
#include "GameEngineCore/Rendering/OpenGL/VertexArray.hpp"

namespace GameEngine {
   
    static bool isOneBuffer = false;
    static bool s_GLFW_initialized = false;

    GLfloat points[] = {
        0.0f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
    };

    GLfloat colors[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
    };

    GLfloat pointsAndColors[] = {
        0.0f, 0.5f, 0.0f,       1.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.0f,      0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 1.0f,
    };

    //in - enter attributes
    //out - output attributes
    const char* vertexShader =
        "#version 460\n"
        "layout(location = 0) in vec3 vertexPosition;"
        "layout(location = 1) in vec3 vertexColor;"
        "out vec3 color;"
        "void main() {"
        "   color = vertexColor;"
        "   gl_Position = vec4(vertexPosition, 1.0);"
        "}";
    
    const char* fragmentShader =
        "#version 460\n"
        "in vec3 color;"
        "out vec4 fragColor;"
        "void main() {"
        " fragColor = vec4(color, 1.0);"
        "}";

    BufferLayout oneElement{
        ShaderDataType::Float3,
        ShaderDataType::Float3
    };
    BufferLayout bufferElementNumberOne {
        ShaderDataType::Float3,
    };

    BufferLayout bufferElementNumberTwo{
        ShaderDataType::Float3,
    };

    GLuint vao;

    std::unique_ptr<ShaderProgram> p_shaderProgram = nullptr;

    std::unique_ptr<VertexBuffer> p_pointsVBO = nullptr;
    std::unique_ptr<VertexBuffer> p_colorsVBO = nullptr;
    
    std::unique_ptr<VertexBuffer> p_pointsAndColorsVBO = nullptr;

    std::unique_ptr<VertexArray> p_twoBuffersVAO = nullptr;
    std::unique_ptr<VertexArray> p_oneBufferVAO = nullptr;

    


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
        p_pointsVBO = std::make_unique<VertexBuffer>(points, sizeof(points), bufferElementNumberOne, VertexBuffer::EUsage::Static);
        p_colorsVBO = std::make_unique<VertexBuffer>(colors, sizeof(colors), bufferElementNumberTwo, VertexBuffer::EUsage::Static);
        p_pointsAndColorsVBO = std::make_unique<VertexBuffer>(pointsAndColors, sizeof(pointsAndColors), oneElement, VertexBuffer::EUsage::Static);
        //generating vertex array object (vertex info container) and connecting to it
        p_twoBuffersVAO = std::make_unique<VertexArray>();
        p_twoBuffersVAO->addBuffer(*p_pointsVBO);
        p_twoBuffersVAO->addBuffer(*p_colorsVBO);

        p_oneBufferVAO = std::make_unique<VertexArray>();
        p_oneBufferVAO->addBuffer(*p_pointsAndColorsVBO);

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

        if (isOneBuffer) {
            p_oneBufferVAO->bind();
        }
        else {
            p_twoBuffersVAO->bind();
        }

        //draw triangle
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawArrays(GL_TRIANGLES, 0, 3);

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
        ImGui::Checkbox("One buffer", &isOneBuffer);
        ImGui::End();



        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(m_pWindow);
        glfwPollEvents();

    }
   
}