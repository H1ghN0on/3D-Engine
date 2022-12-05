#include <GameEngineCore/Renderer.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <GameEngineCore/Log.hpp>
#include "VertexArray.hpp"

namespace GameEngine {

    DrawType Renderer::drawType = DrawType::Triangles;
    int Renderer::polygonModeFront = GL_FILL;
    int Renderer::polygonModeBack = GL_FILL;
    bool Renderer::cullFaceEnabled = false;

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
            type = GL_POINTS;
            break;
        }
        case DrawType::Lines: {
            type = GL_LINES;
            break;
        }
        }

        auto draw = [type, &vertexArray]() {
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
        };

        if (cullFaceEnabled) {
            setCullFaceSide(false);
            glPolygonMode(GL_FRONT_AND_BACK, polygonModeBack);
            draw();
            setCullFaceSide(true);
            glPolygonMode(GL_FRONT_AND_BACK, polygonModeFront);
            draw();
        }
        else {
            draw();
        }
        

        vertexArray.unbind();
    }



    void Renderer::setClearColor(const float r, const float g, const float b, const float a) {
        glClearColor(r, g, b, a);
    }

   

    void Renderer::setPolygonMode(CullType front, CullType back) {
        
        auto CullTypeToGLType = [](CullType value) {
            switch (value) {
            case CullType::Fill: return GL_FILL;
            case CullType::Line: return GL_LINE;
            case CullType::Point: return GL_POINT;
            default: return GL_FILL;
            }
        };

        polygonModeFront = CullTypeToGLType(front);
        polygonModeBack = CullTypeToGLType(back);
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

    void Renderer::enableDepth(bool status) {
        status ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
    }

    void Renderer::enableCullFace(bool status) {
        if (status) {
            glEnable(GL_CULL_FACE);
            cullFaceEnabled = true;
        }
        else {
            glDisable(GL_CULL_FACE);
            cullFaceEnabled = false;
        }
    }

    void Renderer::setCullFaceSide(bool front) {
        front ? glCullFace(GL_FRONT) : glCullFace(GL_BACK);
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