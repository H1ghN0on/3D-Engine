#pragma once

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

namespace GameEngine {
    struct Vertex {

        Vertex(glm::vec3 _position, glm::vec3 _normal, glm::vec2 _texture, glm::vec3 _color = glm::vec3(1.0f, 1.0f, 1.0f))
            : position(_position)
            , normal(_normal)
            , texture(_texture)
            , color(_color)
        {}

        Vertex() {}


        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texture;
        glm::vec3 color;

    };

    template<typename T>
    glm::vec3 toVec3(T x, T y, T z) {
        return glm::vec3(x, y, z);
    }

    template<typename T>
    glm::vec2 toVec2(T x, T y) {
        return glm::vec2(x, y);
    }
}