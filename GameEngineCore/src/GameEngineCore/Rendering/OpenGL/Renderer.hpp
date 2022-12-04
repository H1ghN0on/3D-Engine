#pragma once

#include <GameEngineCore/Enums.hpp>

struct GLFWwindow;

namespace GameEngine {
	class VertexArray;

	enum class BitfieldMask {
		Color, Depth, All
	};

	

	class Renderer {
	public:
		static bool init(GLFWwindow* pWindow);

		static void draw(const VertexArray& vertexArray);

		static void setClearColor(const float r, const float g, const float b, const float a);
		static void clear(BitfieldMask mask);
		static void setViewport(
			const unsigned int width,
			const unsigned int height,
			const unsigned int leftOffset = 0,
			const unsigned int bottomOffset = 0
		);
		static void enableDepth();

		static const char* getVendorStr();
		static const char* getRendererStr();
		static const char* getVersionStr();

		static DrawType drawType;

		static void setDrawType(DrawType _drawType) {
			drawType = _drawType;
		}
	};
}