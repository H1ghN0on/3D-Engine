#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include <memory>

#include <GameEngineCore/Rendering/OpenGL/Model.hpp>
#include <GameEngineCore/Rendering/OpenGL/VertexArray.hpp>
#include <GameEngineCore/Rendering/OpenGL/VertexBuffer.hpp>
#include <GameEngineCore/Rendering/OpenGL/IndexBuffer.hpp>


namespace GameEngine {

	class Object {
	public:
        //Не прокидывается как GLfloat или GLuint
        
        Object(BufferLayout layout, float* vertices, size_t verticesSize, unsigned int* indices = nullptr, unsigned int indicesCount = 0);
        
        Object(const char* modelPath);

        glm::mat4 translate(glm::vec3 position);
        glm::mat4 rotate(float rotation);
        glm::mat4 scale(glm::vec3 scalation);
        glm::mat4 update(glm::vec3 scalation, glm::vec3 position, float rotation);

        std::shared_ptr<VertexArray> getVertexArray();
  

	private:

        std::unique_ptr<Model> model = nullptr;
        std::shared_ptr<VertexArray> vertexArray = nullptr;
        std::unique_ptr<VertexBuffer> vertexBuffer = nullptr;
        std::unique_ptr<IndexBuffer> indexBuffer = nullptr;

	};

}