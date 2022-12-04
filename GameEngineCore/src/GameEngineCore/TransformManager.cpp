#include <GameEngineCore/TransformManager.hpp>


namespace GameEngine {

	std::string TransformManager::activeObjectName = "";

	TransformAxis TransformManager::axis = TransformAxis::X;
	TransformDirection TransformManager::dir = TransformDirection::Forward;
	TransformType TransformManager::type = TransformType::Move;


    glm::vec3 TransformManager::calcTransformVec(float speed) {
        glm::vec3 transformVec = glm::vec3(0.0f, 0.0f, 0.0f);


        if (dir == TransformDirection::Backward) {
            speed *= -1;
        }

        switch (axis) {
        case TransformAxis::X: {
            transformVec.x = speed;
            break;
        }
        case TransformAxis::Y: {
            transformVec.y = speed;
            break;
        }
        case TransformAxis::Z: {
            transformVec.z = speed;
            break;
        }
        }
        return transformVec;
    }

	void TransformManager::transform() {
        
        switch (type) {
        case TransformType::Move: {
            auto pos = ObjectManager::getObject(TransformManager::getActiveObjectName())->getPosition();
            const float speed = 2.5f;
            float trueSpeed = speed * Window::getDeltaTime();

            ObjectManager::getObject(TransformManager::getActiveObjectName())->setPosition(pos + calcTransformVec(trueSpeed));

            break;
        }
        case TransformType::Rotate: {
            auto rot = ObjectManager::getObject(TransformManager::getActiveObjectName())->getRotation();
            const float speed = 35.f;
            float trueSpeed = speed * Window::getDeltaTime();
            ObjectManager::getObject(TransformManager::getActiveObjectName())->setRotation(rot + calcTransformVec(trueSpeed));

            break;
        }
        case TransformType::Scale: {
            auto scal = ObjectManager::getObject(TransformManager::getActiveObjectName())->getScalation();
            const float speed = 2.5f;
            float trueSpeed = speed * Window::getDeltaTime();
           
            ObjectManager::getObject(TransformManager::getActiveObjectName())->setScalation(scal + calcTransformVec(trueSpeed));
            break;
        }
        }
	}

}