#include <soil/src/SOIL.h>
#include <glad/glad.h>
#include <glm/vec4.hpp>
#include "Texture.hpp"
#include "GameEngineCore/Log.hpp"

namespace GameEngine {
	
	constexpr unsigned int wrappingModeToGL(Texture::WrappingMode wrappingMode) {
		switch (wrappingMode) {
			case Texture::WrappingMode::Repeat: return GL_REPEAT;
			case Texture::WrappingMode::MirroredRepeat: return GL_MIRRORED_REPEAT;
			case Texture::WrappingMode::Edge: return GL_CLAMP_TO_EDGE;
			case Texture::WrappingMode::Border: return GL_CLAMP_TO_BORDER;
		}
	}


	constexpr unsigned int mipmapFilterModeToGL(Texture::MipmapFilterMode mipmapFilterMode) {
		switch (mipmapFilterMode) {
			case Texture::MipmapFilterMode::LinearLinear: return GL_LINEAR_MIPMAP_LINEAR;
			case Texture::MipmapFilterMode::LinearNearest: return GL_LINEAR_MIPMAP_NEAREST;
			case Texture::MipmapFilterMode::NearestLinear: return GL_NEAREST_MIPMAP_LINEAR;
			case Texture::MipmapFilterMode::NearestNearest: return GL_NEAREST_MIPMAP_NEAREST;
		}
	}

	
	void Texture::createTexture(const char* fileLocation, WrappingMode wrappingMode, MipmapFilterMode mipmapFilterMode, float const(&borderColor)[4]) {
		unsigned char* image = SOIL_load_image(fileLocation, &m_width, &m_height, 0, SOIL_LOAD_RGB);
		if (!image) {
			LOG_CRITICAL("Image load failed: {0}", SOIL_last_result());
			return;
		}
		glGenTextures(1, &m_id);
		bind();

		if (wrappingMode == WrappingMode::Border) {
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

		}
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrappingModeToGL(wrappingMode));	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrappingModeToGL(wrappingMode));

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mipmapFilterModeToGL(mipmapFilterMode));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mipmapFilterModeToGL(mipmapFilterMode));

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);

		SOIL_free_image_data(image);
		unbind();
	}

	void Texture::bind() {
		glBindTexture(GL_TEXTURE_2D, m_id);
	}

	void Texture::unbind() {
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}