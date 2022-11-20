#pragma once

#include <string>
#include <vector>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <assimp/anim.h>

namespace GameEngine {

    struct KeyPosition
    {
        glm::vec3 position;
        float timeStamp;
    };

    struct KeyRotation
    {
        glm::quat orientation;
        float timeStamp;
    };

    struct KeyScale
    {
        glm::vec3 scale;
        float timeStamp;
    };

    class Bone
    {
    private:
        std::vector<KeyPosition> positions;
        std::vector<KeyRotation> rotations;
        std::vector<KeyScale> scales;
        int positionsNumber;
        int rotationsNumber;
        int scalesNumber;

        glm::mat4 localTransform;
        std::string name;
        int id;


        float getScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime);

        glm::mat4 interpolatePosition(float animationTime);
        glm::mat4 interpolateRotation(float animationTime);
        glm::mat4 interpolateScaling(float animationTime);

    public:
        Bone(const std::string& _name, int _id, const aiNodeAnim* channel);

        void update(float animationTime);

        int getPositionIndex(float animationTime);

        int getRotationIndex(float animationTime);

        int getScaleIndex(float animationTime);

        glm::mat4 getLocalTransform() { return localTransform; }
        std::string getBoneName() const { return name; }
        int getBoneId() { return id; }

    };
}