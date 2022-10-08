#include "Object.hpp"

#include <glm/gtx/string_cast.hpp>
#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>
#include <GameEngineCore/Log.hpp>
#include <glad/glad.h>

#include <iostream>

namespace GameEngine {    

    Object::Object(
        BufferLayout layout, 
        float* vertices, 
        size_t verticesSize, 
        unsigned int* indices, 
        unsigned int indicesCount
    ) {

        vertexArray = std::make_shared<VertexArray>();
        vertexBuffer = std::make_unique<VertexBuffer>(indicesCount, vertices, verticesSize, layout);
        vertexArray->addVertexBuffer(*vertexBuffer);
        if (indices != nullptr) {
            indexBuffer = std::make_unique<IndexBuffer>(indices, indicesCount);
            vertexArray->setIndexBuffer(*indexBuffer);
        }
    }

    Object::Object(
        const char* modelPath
    ) {
        model = std::make_unique<Model>(modelPath);
    }

    glm::mat4 Object::scale(glm::vec3 scalation) {
        return glm::mat4(
            scalation[0], 0, 0, 0,
            0, scalation[1], 0, 0,
            0, 0, scalation[2], 0,
            0, 0, 0, 1
        );
    }

    glm::mat4 Object::translate(glm::vec3 position) {

        return glm::mat4 (
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            position[0], position[1], position[2], 1
        );
    }

    glm::mat4 Object::rotate(float rotation) {
        float radiansRotate = glm::radians(rotation);
       
        return glm::mat4 (
            cos(radiansRotate), sin(radiansRotate), 0, 0,
            -sin(radiansRotate), cos(radiansRotate), 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        );
    }

    glm::mat4 Object::update(glm::vec3 scalation, glm::vec3 position, float rotation) {

        return translate(position) * rotate(rotation) * scale(scalation);
	}

    std::shared_ptr<VertexArray> Object::getVertexArray() {
        return vertexArray;
    }

}