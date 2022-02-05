#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/mat3x3.hpp>
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


        glm::mat3 mat1(4, 0, 0, 2, 8, 1, 0, 1, 0);
        glm::mat3 mat2(4, 2, 9, 2, 0, 4, 1, 4, 2);

        glm::mat3 result = mat1 * mat2;

        LOG_INFO("");
        LOG_INFO("|{0:3} {1:3} {2:3}|", result[0][0], result[1][0], result[2][0]);
        LOG_INFO("|{0:3} {1:3} {2:3}|", result[0][1], result[1][1], result[2][1]);
        LOG_INFO("|{0:3} {1:3} {2:3}|", result[0][2], result[1][2], result[2][2]);
        LOG_INFO("");

        glm::vec4 vec(1, 2, 3, 4);
        glm::mat4 matIdentity(1);
        
        glm::vec4 resultVec = matIdentity * vec;
        LOG_INFO("|{0} {1} {2} {3}|", resultVec.x, resultVec.y, resultVec.z, resultVec.w);
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
        ImGui::End();



        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(m_pWindow);
        glfwPollEvents();

    }
   
}