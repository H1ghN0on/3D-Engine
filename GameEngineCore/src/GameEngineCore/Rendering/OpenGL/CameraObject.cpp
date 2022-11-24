#include "CameraObject.hpp"
#include <iostream>
#include <glm/trigonometric.hpp>


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

	void CameraObject::translate(Direction dir, float deltaTime) {
		float trueSpeed = speed * deltaTime;
		switch(dir) {

			case Direction::Forward: {
				position += trueSpeed * glm::vec3(front.x, 0.f, front.z);
				break;
			}

			case Direction::Back: {
				position -= trueSpeed * glm::vec3(front.x, 0.f, front.z);
				break;
			}

			case Direction::Left: {
				position -= glm::normalize(glm::cross(front, up)) * trueSpeed;
				break;
			}

			case Direction::Right: {
				position += glm::normalize(glm::cross(front, up)) * trueSpeed;
				break;
			}

			case Direction::Up: {
				position += up * trueSpeed;
				break;
			}
			case Direction::Down: {
				if (position.y > 1) {
					position -= up * trueSpeed;
				}
				
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
	

		if (rotation[0] > 90)
			rotation[0] = 90;
		if (rotation[0] < -90.f)
			rotation[0] = -90.f;

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