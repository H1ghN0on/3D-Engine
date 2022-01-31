#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "GameEngineCore/Window.hpp"
#include "GameEngineCore/Log.hpp"

namespace GameEngine {
   
    static bool s_GLFW_initialized = false;

	Window::Window(const unsigned int width, const unsigned int height, std::string title):
        m_width(width),
        m_height(height),
        m_title(std::move(title))
    {
		int resultCode = init();
	}
	Window::~Window() {
		shutdown();
	}

	int Window::init() {

        LOG_INFO("Creating window '{0}', {1}x{2}", m_title, m_width, m_height);

        if (!s_GLFW_initialized) {
            if (!glfwInit()) {
                LOG_CRITICAL("Failed to initialize GLFW");
                return -1;
            }
            s_GLFW_initialized = true;
        }

        m_pWindow = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
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

        return 0;

	}
    void Window::shutdown() {
        glfwDestroyWindow(m_pWindow);
        glfwTerminate();
    }

    void Window::on_update() {

        glClearColor(1, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(m_pWindow);
        glfwPollEvents();

    }
   
}