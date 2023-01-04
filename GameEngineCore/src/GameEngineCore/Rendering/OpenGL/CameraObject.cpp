#include "CameraObject.hpp"
#include <iostream>
#include <glm/trigonometric.hpp>
#include "GameEngineCore/Window.hpp"

namespace GameEngine {
	CameraObject::CameraObject(
		glm::vec3 _position,
		glm::vec3 _rotation,
		ProjectionType _type,
		glm::vec3 _front
	)
		: front(_front)
		, position(_position)
		, rotation(_rotation)
		, type(_type)
	{
		updateViewMatrix(position, position + front, up);
		updateProjectionMatrix(type);
	}

	void CameraObject::setPosition(glm::vec3 _position) {
		position = _position;
		updateViewMatrix(position, position + front, up);
	}

	void CameraObject::translate(CameraDirection dir) {
		float trueSpeed = speed * Window::getDeltaTime();
		switch(dir) {

			case CameraDirection::Forward: {
				position += trueSpeed * glm::vec3(front.x, 0.f, front.z);
				break;
			}

			case CameraDirection::Back: {
				position -= trueSpeed * glm::vec3(front.x, 0.f, front.z);
				break;
			}

			case CameraDirection::Left: {
				position -= glm::normalize(glm::cross(front, up)) * trueSpeed;
				break;
			}

			case CameraDirection::Right: {
				position += glm::normalize(glm::cross(front, up)) * trueSpeed;
				break;
			}

			case CameraDirection::Up: {
				position += up * trueSpeed;
				break;
			}
			case CameraDirection::Down: {
				position -= up * trueSpeed;
				break;
			}
		}
	
		updateViewMatrix(position, position + front, up);
	}

	void CameraObject::rotate(float x, float y) {
	
		float xoffset = x - lastX;
		float yoffset = lastY - y;
		lastX = x;
		lastY = y;

		xoffset *= sensitivity;
		yoffset *= sensitivity;
		rotation[1] += xoffset;
		rotation[0] += yoffset;
	

		if (rotation[0] > 89)
			rotation[0] = 89;
		if (rotation[0] < -89.f)
			rotation[0] = -89.f;

		front.x = cos(glm::radians(rotation[0])) * cos(glm::radians(rotation[1]));
		front.y = sin(glm::radians(rotation[0]));
		front.z = cos(glm::radians(rotation[0])) * sin(glm::radians(rotation[1]));
		front = glm::normalize(front);

		updateViewMatrix(position, position + front, up);

	}

	void CameraObject::setType(ProjectionType _type) {
		type = _type;
		updateProjectionMatrix(type);
	}

	glm::mat4 CameraObject::update() {
		return getProjectionMatrix() * getViewMatrix();
	}


}