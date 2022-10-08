#pragma once

#include <string>

namespace GameEngine {

	class Texture {
	public:
		enum class WrappingMode {
			Repeat,
			MirroredRepeat,
			Edge,
			Border,
		};

		enum class Type {
			Diffusal, Specular,
		};

		enum class MipmapFilterMode {
			LinearNearest,
			LinearLinear,
			NearestLinear,
			NearestNearest,
		};

		Texture(std::string fileLocation, Type type, WrappingMode wrappingMode, MipmapFilterMode mipmapFilterMode, float const(&borderColor)[4] = { 0, 0, 0, 0 });

		void bind();
		void unbind();

		
		std::string getLocation() {
			return fileLocation;
		}
		unsigned int getId() {
			return m_id;
		}
		Type getType() {
			return type;
		}

	private:
		Type type;
		std::string fileLocation;
		unsigned int m_id;
		int m_width, m_height;
	};
}