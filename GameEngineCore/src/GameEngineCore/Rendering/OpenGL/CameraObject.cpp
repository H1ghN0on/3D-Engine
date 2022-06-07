#include "CameraObject.hpp"
#include <iostream>


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
		updateViewMatrix(position, rotation);
		updateProjectionMatrix(type);
	}

	void CameraObject::move(Direction dir, float deltaTime) {
		float trueSpeed = speed * deltaTime;
		switch(dir) {

			case Direction::Forward: {
				position += trueSpeed * front;
				break;
			}

			case Direction::Back: {
				position -= trueSpeed * front;
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
				position -= up * trueSpeed;
				break;
			}
		}
		updateViewMatrix(position, rotation);
	}

	void CameraObject::rotate(float x, float y) {

		float xoffset = lastX - x;
		float yoffset = lastY - y;
		lastX = x;
		lastY = y;

		xoffset *= sensitivity;
		yoffset *= sensitivity;
		rotation[0] += yoffset;
		rotation[1] += xoffset;

		if (rotation[0] > 90)
			rotation[0] = 90;
		if (rotation[0] < -90.f)
			rotation[0] = -90.f;

		
		updateViewMatrix(position, rotation);

		/* todo - идти в направлении взляда */
	}

	void CameraObject::setType(ProjectionType _type) {
		type = _type;
		updateProjectionMatrix(type);
	}

	void CameraObject::update() {
		glm::mat4 viewAndProjectionMatrix = getProjectionMatrix() * getViewMatrix();
	}


}