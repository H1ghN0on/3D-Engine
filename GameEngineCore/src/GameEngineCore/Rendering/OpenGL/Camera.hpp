#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace GameEngine {

	class Camera {
	public:

		enum class ProjectionType {
			Perspective,
			Orthographic
		};

		Camera() = default;
		Camera(glm::vec3 position, glm::vec3 rotaition, ProjectionType type);

		void setPosition(glm::vec3 position);
		void setRotation(glm::vec3 rotation);
		void setPositionAndRotation(glm::vec3 position, glm::vec3 rotation);
		void setType(Camera::ProjectionType type);

		glm::mat4x4 getViewMatrix() const { return m_viewMatrix; }
		glm::mat4x4 getProjectionMatrix() const { return m_projectionMatrix; }
	private:
		void updateViewMatrix();
		void updateProjectionMatrix();

		glm::vec3 m_position;
		glm::vec3 m_rotation;
		ProjectionType m_type;

		glm::mat4x4 m_viewMatrix;
		glm::mat4x4 m_projectionMatrix;

	};
}