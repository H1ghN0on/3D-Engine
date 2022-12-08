#include "Object.hpp"

#include <glm/gtx/string_cast.hpp>
#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>
#include <GameEngineCore/Log.hpp>
#include <glad/glad.h>

#include <iostream>

namespace GameEngine {    

   Object::Object(
        std::vector<Vertex> vertices,
        std::vector<unsigned int> indices,
        std::vector<Texture> textures,
        glm::vec3 _position,
        glm::vec3 _scalation,
        glm::vec3 _rotation
    ) 
    : position(_position)
    , scalation(_scalation)
    , rotation(_rotation) 
    {

        model = std::make_unique<Model>(vertices, indices, textures);
    }

    Object::Object(
        const char* modelPath,
        glm::vec3 _position,
        glm::vec3 _scalation,
        glm::vec3 _rotation
    )
    : position(_position)
    , scalation(_scalation)
    , rotation(_rotation)
    {


        model = std::make_unique<Model>(modelPath);
    }

    glm::mat4 Object::scale() {
        return glm::mat4(
            scalation[0], 0, 0, 0,
            0, scalation[1], 0, 0,
            0, 0, scalation[2], 0,
            0, 0, 0, 1
        );
    }

    glm::mat4 Object::translate() {

        return glm::mat4 (
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            position[0], position[1], position[2], 1
        );
    }

    glm::mat4 Object::rotate() {
        float radiansRotateX = glm::radians(rotation.x);
        float radiansRotateY = glm::radians(rotation.y);
        float radiansRotateZ = glm::radians(rotation.z);
       
        glm::mat4 rotateZ = glm::mat4 (
            cos(radiansRotateZ), sin(radiansRotateZ), 0, 0,
            -sin(radiansRotateZ), cos(radiansRotateZ), 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        );

        glm::mat4 rotateY = glm::mat4(
            cos(radiansRotateY), 0, sin(radiansRotateY), 0,
            0, 1, 0, 0,
            -sin(radiansRotateY), 0, cos(radiansRotateY), 0,
            0, 0, 0, 1
        );

        glm::mat4 rotateX = glm::mat4(
            1, 0, 0, 0,
            0, cos(radiansRotateX), -sin(radiansRotateX), 0,
            0, sin(radiansRotateX), cos(radiansRotateX), 0,
            0, 0, 0, 1
        );

        return rotateZ * rotateY * rotateX;

    }

    glm::mat4 Object::update() {

        return translate() * rotate() * scale();
	}

    void Object::setShader(std::shared_ptr<ShaderProgram> _shader)
    {
        shader = _shader;
    }

    void Object::setMaterial(ShaderMaterial _material) {
        material = _material;
    }


    void Object::draw() {
     
        shader->bind();
        shader->setMatrix4("transformMatrix", update());
        model->draw(shader);
    
    }

    std::shared_ptr<VertexArray> Object::getVertexArray() {
        return vertexArray;
    }




 

}