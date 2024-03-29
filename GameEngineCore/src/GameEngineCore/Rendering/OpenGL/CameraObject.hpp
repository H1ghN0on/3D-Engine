#pragma once

#include "Camera.hpp"

#include <glm/vec3.hpp>

namespace GameEngine {


	class CameraObject: public Camera {
	public:

		

		CameraObject() = default;
		CameraObject(
			glm::vec3 _position,
			glm::vec3 _rotation,
			ProjectionType type,
			glm::vec3 _front = { 0.f, 0.f, -1.f }
		);

		void translate(CameraDirection dir);
		void rotate(float x, float y);
		glm::mat4 update();
		void setType(ProjectionType type);
		glm::vec3 getFront() const { return front;  }
		glm::vec3 getPosition() const { return position; }

		void setPosition(glm::vec3 _position);

		void setSpeed(float _speed) { speed = _speed; }
	private:

		ProjectionType type;

		glm::vec3 front = { 0.f, 0.f, -1.f };
		glm::vec3 up = { 0.0f, 1.0f, 0.0f };
	

		glm::vec3 position = { 0.f, 0.f, 0.f };
		glm::vec3 rotation = { 0.f, 0.f, 0.f };

		float speed = 5.5f;
		const float sensitivity = 0.05;

		float lastX = 400, lastY = 300;
	};

}