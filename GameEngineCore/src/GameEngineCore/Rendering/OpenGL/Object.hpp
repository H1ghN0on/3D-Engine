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
        
        Object(
            std::vector<GLfloat> vertices,
            std::vector<unsigned int> indices,
            std::vector<Texture> textures,
            bool hasNormals,
            glm::vec3 _position,
            glm::vec3 _scalation,
            float _rotation
        );
        
        Object(const char* modelPath, glm::vec3 _position, glm::vec3 _scalation, float _rotation);

        glm::mat4 update();

        void setShader(std::shared_ptr<ShaderProgram> shader);

        std::shared_ptr<VertexArray> getVertexArray();
        
        void setPosition(glm::vec3 _position) {
            position = _position;
        }
        void setRotation(float _rotation) {
            rotation = _rotation;
        }
        void setScalation(glm::vec3 _scalation) {
            scalation = _scalation;
        }

        glm::vec3 getPosition() {
            return position;
        }

        void draw(glm::mat4 viewAndProjectionMatrix);
	private:

        glm::mat4 translate();
        glm::mat4 rotate();
        glm::mat4 scale();

        glm::vec3 scalation;
        glm::vec3 position;
        float rotation;

        std::shared_ptr<ShaderProgram> shader = nullptr;
        std::unique_ptr<Model> model = nullptr;
        std::shared_ptr<VertexArray> vertexArray = nullptr;
        std::unique_ptr<VertexBuffer> vertexBuffer = nullptr;
        std::unique_ptr<IndexBuffer> indexBuffer = nullptr;

	};

}