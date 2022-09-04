#pragma once

#include <glm/mat4x4.hpp>

namespace GameEngine {

	class ShaderProgram {
	public:

		ShaderProgram(const char* vertexShaderSrc, const char* fragmentShaderSrc);
		ShaderProgram(ShaderProgram&&);
		ShaderProgram& operator=(ShaderProgram&&);
		~ShaderProgram();

		ShaderProgram() = delete;
		ShaderProgram(const ShaderProgram&) = delete;
		ShaderProgram& operator=(const ShaderProgram&) = delete;

		void bind() const;
		static void unbind();

		unsigned int get();

		bool isCompiled() const { return m_isCompiled; }
		void setMatrix4(const char* name, const glm::mat4& matrix) const;

	private:
		bool m_isCompiled = false;
		unsigned int m_id = 0;
	};
}