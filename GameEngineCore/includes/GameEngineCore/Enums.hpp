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
		Triangles, TrianglesFan, Points, Lines, TriangleStrip
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
		CYAN_PLASTIC,
		CHROME,
		DEFAULT,
		NONE,
	};
}