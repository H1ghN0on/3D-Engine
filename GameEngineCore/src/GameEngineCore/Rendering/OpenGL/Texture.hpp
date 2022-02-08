#pragma once

namespace GameEngine {

	class Texture {
	public:
		enum class WrappingMode {
			Repeat,
			MirroredRepeat,
			Edge,
			Border,
		};

		enum class MipmapFilterMode {
			LinearNearest,
			LinearLinear,
			NearestLinear,
			NearestNearest,
		};

		Texture() = default;

		void createTexture(const char* fileLocation, WrappingMode wrappingMode, MipmapFilterMode mipmapFilterMode, float const(&borderColor)[4] = { 0, 0, 0, 0 });

		void bind();
		void unbind();
	private:
		unsigned int m_id;
		int m_width, m_height;
	};
}