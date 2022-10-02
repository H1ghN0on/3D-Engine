#pragma once

#include <glm/mat4x4.hpp>
#include <map>
#include <vector>
#include <any>
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


		enum class PropertyTypes {
			Vec3, Vec4, Float, Int, Mat3, Mat4
		};

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
		void setInt(const char* name, const int value) const;

		
		void setObject(const char* name, std::map<char*, std::pair<PropertyTypes, std::any>> properties) const;
		void setObjects(const char* name, std::vector<std::map<char*, std::pair<PropertyTypes, std::any>>>) const;


	private:
		bool m_isCompiled = false;
		unsigned int m_id = 0;
	};
}