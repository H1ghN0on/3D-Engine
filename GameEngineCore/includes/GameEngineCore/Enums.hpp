#pragma once


namespace GameEngine {
	enum class ProjectionType {
		Perspective,
		Orthographic
	};

	enum class CameraDirection {
		Up, Down, Left, Right, Forward, Back
	};

	enum class DrawType {
		Triangles, TrianglesFan, Points, Lines
	};

	enum class CullType {
		Fill, Line, Point
	};

	enum class ShaderMaterial {
		EMERALD,
		JADE,
		PEARL,
		GOLD,
		RUBBER,
		DEFAULT,
		NONE,
	};
}