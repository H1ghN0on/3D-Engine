#include <glm/trigonometric.hpp>

#include "Camera.hpp"
#include "GameEngineCore/Window.hpp"
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

namespace GameEngine {


	void Camera::updateViewMatrix(glm::vec3 position, glm::vec3 front, glm::vec3 up) {

		glm::vec3 frontVector = glm::normalize(position - front);

		glm::vec3 rightVector = glm::normalize(glm::cross(up, frontVector));


		glm::vec3 upVector = glm::cross(frontVector, rightVector);

		glm::mat4 orientation = {
		   glm::vec4(rightVector.x, upVector.x, frontVector.x, 0),
		   glm::vec4(rightVector.y, upVector.y, frontVector.y, 0),
		   glm::vec4(rightVector.z, upVector.z, frontVector.z, 0),
		   glm::vec4(0, 0, 0, 1)


		};

		glm::mat4 translation = {
		   glm::vec4(1, 0, 0, 0),
		   glm::vec4(0, 1, 0, 0),
		   glm::vec4(0, 0, 1, 0),
		   glm::vec4(-position.x, -position.y, -position.z, 1)
		};
		m_viewMatrix = orientation * translation;
	}

	void Camera::updateProjectionMatrix(ProjectionType _type) {
		//projection

		float f = 100;
		float n = 0.1f;

		const auto &screenSize = Window::getScreenSize();

		if (_type == ProjectionType::Perspective) {
			m_projectionMatrix = glm::perspective(
				glm::radians(90.f),
				screenSize.x / screenSize.y,
				n,
				f
			);
		}
		else {
			m_projectionMatrix = glm::ortho(
				-1 * screenSize.x / screenSize.y,
				1 * screenSize.x / screenSize.y,
				-1.f,
				1.f,
				n,
				f
			);
		}

		/*if (_type == ProjectionType::Orthographic) {
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
			float f = 50;
			float n = 0.1f;
			m_projectionMatrix = glm::mat4(n / r, 0, 0, 0,
				0, n / t, 0, 0,
				0, 0, (-f - n) / (f - n), -1,
				0, 0, -2 * f * n / (f - n), 0);
		}*/

	}

	//void Camera::setPosition(glm::vec3 position) {
	//	m_position = position;
	//	updateViewMatrix();
	//}

	//void Camera::setRotation(glm::vec3 rotation) {
	//	m_rotation = rotation;
	//	updateViewMatrix();
	//}

	//void Camera::setType(Camera::ProjectionType type) {
	//	m_type = type;
	//	updateProjectionMatrix();
	//}


	//void Camera::setPositionAndRotation(glm::vec3 position, glm::vec3 rotation) {
	//	m_position = position;
	//	m_rotation = rotation;
	//	updateViewMatrix();
	//}

}