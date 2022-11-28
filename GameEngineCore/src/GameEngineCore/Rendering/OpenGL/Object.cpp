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
        float _rotation
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
        float _rotation
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
        float radiansRotate = glm::radians(rotation);
       
        return glm::mat4 (
            cos(radiansRotate), sin(radiansRotate), 0, 0,
            -sin(radiansRotate), cos(radiansRotate), 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        );
    }

    glm::mat4 Object::update() {

        return translate() * rotate() * scale();
	}

    void Object::setShader(std::shared_ptr<ShaderProgram> _shader)
    {
        shader = _shader;
    }



    void Object::draw(glm::mat4 viewAndProjectionMatrix) {
      
    

        shader->bind();
        //shader->setMatrix4("viewAndProjectionMatrix", viewAndProjectionMatrix);
        shader->setMatrix4("transformMatrix", update());
        model->draw(shader);

        
    
    }

    std::shared_ptr<VertexArray> Object::getVertexArray() {
        return vertexArray;
    }




 

}