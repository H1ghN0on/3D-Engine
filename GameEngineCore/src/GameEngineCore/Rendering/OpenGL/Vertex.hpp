#pragma once

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

namespace GameEngine {
    struct Vertex {

        Vertex(glm::vec3 _position, glm::vec3 _normal, glm::vec2 _texture)
            : position(_position)
            , normal(_normal)
            , texture(_texture)
        {}


        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texture;
    };
}