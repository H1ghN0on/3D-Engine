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

		glm::mat4x4 getViewMatrix() const { return m_viewMatrix; }
		glm::mat4x4 getProjectionMatrix() const { return m_projectionMatrix; }
	protected:
		void updateViewMatrix(glm::vec3 _positionVec, glm::vec3 front, glm::vec3 up);
		void updateProjectionMatrix(ProjectionType _type);

		glm::mat4x4 m_viewMatrix;
		glm::mat4x4 m_projectionMatrix;


	};
}