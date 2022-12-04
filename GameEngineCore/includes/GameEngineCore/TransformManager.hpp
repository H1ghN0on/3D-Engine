#pragma once

#include <string>
#include <GameEngineCore/ObjectManager.hpp>
#include <GameEngineCore/Window.hpp>

namespace GameEngine {


	enum class TransformAxis {
		X, Y, Z
	};

	enum class TransformDirection {
		Forward, Backward
	};

	enum class TransformType {
		Move, Rotate, Scale
	};


	static class TransformManager {
	public:

		static std::string getActiveObjectName() { return activeObjectName; }
		static void updateActiveObjectName(std::string name) { activeObjectName = name; }

		static void updateType(TransformType name) { type = name; }

		static void updateAxis(TransformAxis name) { axis = name; }

		static void updateDirection(TransformDirection name) { dir = name; }

		static void transform();

		
	private:
		static std::string activeObjectName;
		static TransformAxis axis;
		static TransformDirection dir;
		static TransformType type;

		static glm::vec3 calcTransformVec(float speed);
	};
}