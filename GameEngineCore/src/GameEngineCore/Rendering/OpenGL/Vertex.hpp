#pragma once

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#define MAX_BONE_INFLUENCE 4

namespace GameEngine {
    struct Vertex {

        Vertex(glm::vec3 _position, glm::vec3 _normal, glm::vec2 _texture, glm::vec3 _color = glm::vec3(1.0f, 1.0f, 1.0f))
            : position(_position)
            , normal(_normal)
            , texture(_texture)
            , color(_color)
        {}


        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texture;
        glm::vec3 color;

    };
}