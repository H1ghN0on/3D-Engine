#include "Renderer.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <GameEngineCore/Log.hpp>
#include "VertexArray.hpp"

namespace GameEngine {

    DrawType Renderer::drawType = DrawType::Triangles;

	bool Renderer::init(GLFWwindow* pWindow) {
        glfwMakeContextCurrent(pWindow);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            LOG_CRITICAL("Failed to initialize GLAD");
            return false;
        }
        LOG_INFO("OpenGL context initialized");
        LOG_INFO(" Vendor: {0}", getVendorStr());
        LOG_INFO(" Renderer: {1}", getRendererStr());
        LOG_INFO(" Version: {2}", getVersionStr());

        return true;
	}

    void Renderer::draw(const VertexArray& vertexArray) {
        vertexArray.bind();
        
        int type;

        switch (drawType) {
        case DrawType::Triangles: {
            type = GL_TRIANGLES;
            break;
        }
        case DrawType::TrianglesFan: {
            type = GL_TRIANGLE_FAN;
            break;
        }
        case DrawType::Points: {
            type = GL_POINT;
            break;
        }
        }

        if (!vertexArray.hasIndexBuffer()) {
            //ренедер при рисовании не по индексам
            glDrawArrays(type, 0, vertexArray.getVerticesCount());
        }
        else {
            glDrawElements(
                type,
                static_cast<GLsizei>(vertexArray.getIndicesCount()),
                GL_UNSIGNED_INT,
                nullptr
            );
        }

       
        

        vertexArray.unbind();
    }



    void Renderer::setClearColor(const float r, const float g, const float b, const float a) {
        glClearColor(r, g, b, a);
    }

    void Renderer::clear(BitfieldMask mask) {
        if (mask == BitfieldMask::Color) {
            glClear(GL_COLOR_BUFFER_BIT);
        }
        else if (mask == BitfieldMask::Depth) {
            glClear(GL_DEPTH_BUFFER_BIT);
        }
        else if (mask == BitfieldMask::All) {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }
        
    }

    void Renderer::setViewport(
        const unsigned int width,
        const unsigned int height,
        const unsigned int leftOffset,
        const unsigned int bottomOffset
    ) {
        glViewport(leftOffset, bottomOffset, width, height);
    }

    void Renderer::enableDepth() {
        glEnable(GL_DEPTH_TEST);
    }

    const char* Renderer::getVendorStr() {
        return reinterpret_cast<const char*>(glGetString(GL_VENDOR));
    }
    const char* Renderer::getRendererStr() {
        return reinterpret_cast<const char*>(glGetString(GL_RENDERER));
    }
    const char* Renderer::getVersionStr() {
        return reinterpret_cast<const char*>(glGetString(GL_VERSION));
    }
}