#pragma once

#include <glm/mat4x4.hpp>

namespace GameEngine {
    struct BoneInfo
    {
        /*id is index in finalBoneMatrices*/
        int id;

        /*offset matrix transforms vertex from model space to bone space*/
        glm::mat4 offset;

    };
}