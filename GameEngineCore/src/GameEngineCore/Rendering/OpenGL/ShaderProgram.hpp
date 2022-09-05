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

		void setMatrix3(const char* name, const glm::mat3& matrix) const;
		void setMatrix4(const char* name, const glm::mat4& matrix) const;

		void setVec2(const char* name, const glm::vec2& vector) const;
		void setVec3(const char* name, const glm::vec3& vector) const;
		void setVec4(const char* name, const glm::vec4& vector) const;

		void setFloat(const char* name, const float value) const;

	private:
		bool m_isCompiled = false;
		unsigned int m_id = 0;
	};
}