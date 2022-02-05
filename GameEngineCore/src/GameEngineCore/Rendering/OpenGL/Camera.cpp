#include <glm/trigonometric.hpp>

#include "Camera.hpp"

namespace GameEngine {
	Camera::Camera(glm::vec3 position, glm::vec3 rotation, Camera::ProjectionType type)
		: m_position(position)
		, m_rotation(rotation)
		, m_type(type)
	{
		updateViewMatrix();
		updateProjectionMatrix();
	}

	void Camera::updateViewMatrix() {
		glm::mat4 translateMatrix(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			-m_position[0], -m_position[1], -m_position[2], 1
		);
		float rotationInRadians_x = glm::radians(-m_rotation.x);
		glm::mat4 rotationMatrix_x(
			1, 0, 0, 0,
			0, cos(rotationInRadians_x), sin(rotationInRadians_x), 0,
			0, -sin(rotationInRadians_x), cos(rotationInRadians_x), 0,
			0, 0, 0, 1
		);
		float rotationInRadians_y = glm::radians(-m_rotation.y);
		glm::mat4 rotationMatrix_y(
			cos(rotationInRadians_y), 0, -sin(rotationInRadians_y), 0,
			0, 1, 0, 0,
			sin(rotationInRadians_y), 0, cos(rotationInRadians_y), 0,
			0, 0, 0, 1
		);
		float rotationInRadians_z = glm::radians(-m_rotation.z);
		glm::mat4 rotationMatrix_z(
			cos(rotationInRadians_z), sin(rotationInRadians_z), 0, 0,
			-sin(rotationInRadians_z), cos(rotationInRadians_z), 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		);

		m_viewMatrix = rotationMatrix_y * rotationMatrix_x  * translateMatrix;
	}

	void Camera::updateProjectionMatrix() {
		//projection
		if (m_type == ProjectionType::Orthographic) {
			float r = 2;
			float t = 2;
			float f = 100;
			float n = 0.1f;
			m_projectionMatrix = glm::mat4(
				1 / r, 0, 0, 0,
				0, 1 / t, 0, 0,
				0, 0, -2 / (f - n), 0,
				0, 0, (-f - n) / (f - n), 1);
		}
		else {
			float r = 0.1f;
			float t = 0.1f;
			float f = 10;
			float n = 0.1f;
			m_projectionMatrix = glm::mat4(n / r, 0, 0, 0,
				0, n / t, 0, 0,
				0, 0, (-f - n) / (f - n), -1,
				0, 0, -2 * f * n / (f - n), 0);
		}

	}

	void Camera::setPosition(glm::vec3 position) {
		m_position = position;
		updateViewMatrix();
	}

	void Camera::setRotation(glm::vec3 rotation) {
		m_rotation = rotation;
		updateViewMatrix();
	}

	void Camera::setType(Camera::ProjectionType type) {
		m_type = type;
		updateProjectionMatrix();
	}


	void Camera::setPositionAndRotation(glm::vec3 position, glm::vec3 rotation) {
		m_position = position;
		m_rotation = rotation;
		updateViewMatrix();
	}

}