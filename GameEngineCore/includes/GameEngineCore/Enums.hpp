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
		Triangles, TrianglesFan, Points
	};
}