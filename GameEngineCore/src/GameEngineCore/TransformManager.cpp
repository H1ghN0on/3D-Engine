#include <GameEngineCore/TransformManager.hpp>
#include <GameEngineCore/ObjectManager.hpp>
#include <GameEngineCore/Window.hpp>

namespace GameEngine {

	std::string TransformManager::activeObjectName = "";

	TransformAxis TransformManager::axis = TransformAxis::X;
	TransformDirection TransformManager::dir = TransformDirection::Forward;
	TransformType TransformManager::type = TransformType::Move;


	void TransformManager::transform() {
        const float speed = 2.5f;
        float trueSpeed = speed * Window::getDeltaTime();

        

        switch (type) {
        case TransformType::Move: {

            auto pos = ObjectManager::getObject(TransformManager::getActiveObjectName())->getPosition();

            glm::vec3 translateVec = glm::vec3(0.0f, 0.0f, 0.0f);

           
            if (dir == TransformDirection::Backward) {
                trueSpeed *= -1;
            }

            switch (axis) {
            case TransformAxis::X: {
                translateVec.x = trueSpeed;
                break;
            }
            case TransformAxis::Y: {
                translateVec.y = trueSpeed;
                break;
            }
            case TransformAxis::Z: {
                translateVec.z = trueSpeed;
                break;
            }
            }

            ObjectManager::getObject(TransformManager::getActiveObjectName())->setPosition(pos + translateVec);

            break;
        }
        case TransformType::Rotate: {
            break;
        }
        case TransformType::Scale: {
            break;
        }
        }
	}

}