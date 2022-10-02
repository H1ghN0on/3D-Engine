#include "ShaderProgram.hpp";
#include "GameEngineCore/Log.hpp";

#include <fstream>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
 
namespace GameEngine {



	std::string readShader(const char* filepath) {
		std::string shader;
		std::string allShadersPath = "../../GameEngineCore/shaders/";
		std::ifstream shaderFile(allShadersPath.append(filepath));
		std::stringstream ss;
		if (!shaderFile) {
			LOG_CRITICAL("Shader file has not been opened: {0}", filepath);
		}
		ss << shaderFile.rdbuf();
		return ss.str();
	}


	bool createShader(const char* source, const GLenum shaderType, GLuint& shaderId) {
		//create shader
		shaderId = glCreateShader(shaderType);
		//link src to shader
		glShaderSource(shaderId, 1, &source, nullptr);
		//compile shader
		glCompileShader(shaderId);

		//check if shader compiled
		GLint success;
		//get the status
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);

		if (success == GL_FALSE) {
			char infoLog[1024];
			//log of error
			glGetShaderInfoLog(shaderId, 1024, nullptr, infoLog);

			LOG_CRITICAL("Shader compilation error:\n{}", infoLog);
			return false;
		}
		return true;
	}



	ShaderProgram::ShaderProgram(const char* vertexShaderPath, const char* fragmentShaderPath) {
		//creating vertex and fragment shaders
		std::string vertexShaderSrc = readShader(vertexShaderPath);
		std::string fragmentShaderSrc = readShader(fragmentShaderPath);
		GLuint vertexShaderId = 0;
		if (!createShader(vertexShaderSrc.c_str(), GL_VERTEX_SHADER, vertexShaderId)) {
			LOG_CRITICAL("VERTEX SHADER: compile-time error!");
			glDeleteShader(vertexShaderId);
			return;
		}

		GLuint fragmentShaderId = 0;
		if (!createShader(fragmentShaderSrc.c_str(), GL_FRAGMENT_SHADER, fragmentShaderId)) {
			LOG_CRITICAL("FRAGMENT SHADER: compile-time error!");
			glDeleteShader(vertexShaderId);
			glDeleteShader(fragmentShaderId);
			return;
		}

		//creating the united shader program
		m_id = glCreateProgram();
		glAttachShader(m_id, vertexShaderId);
		glAttachShader(m_id, fragmentShaderId);
		glLinkProgram(m_id);
		//check if program compiled

		GLint success;
		glGetProgramiv(m_id, GL_COMPILE_STATUS, &success);

		if (success == GL_FALSE) {
			char infoLog[1024];
			glGetShaderInfoLog(m_id, 1024, nullptr, infoLog);
			LOG_CRITICAL("Shader Program compilation error:\n{}", infoLog);
			m_isCompiled = false;
			m_id = 0;
			return;
		}
		else {
			m_isCompiled = true;
		}
		//detaching and deleting shaders due to uselessness
		glDetachShader(m_id, vertexShaderId);
		glDetachShader(m_id, fragmentShaderId);
		glDeleteShader(vertexShaderId);
		glDeleteShader(fragmentShaderId);

	}

	ShaderProgram::~ShaderProgram()
	{
		glDeleteProgram(m_id);
	}

	//setting the uniform variable

	void ShaderProgram::setMatrix3(const char* name, const glm::mat3& matrix) const
	{
		glUniformMatrix3fv(glGetUniformLocation(m_id, name), 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void ShaderProgram::setMatrix4(const char* name, const glm::mat4& matrix) const
	{
		glUniformMatrix4fv(glGetUniformLocation(m_id, name), 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void ShaderProgram::setVec2(const char* name, const glm::vec2& vector) const
	{
		glUniform2fv(glGetUniformLocation(m_id, name), 1, glm::value_ptr(vector));
	}

	void ShaderProgram::setVec3(const char* name, const glm::vec3& vector) const
	{
		glUniform3fv(glGetUniformLocation(m_id, name), 1, glm::value_ptr(vector));
	}

	void ShaderProgram::setVec4(const char* name, const glm::vec4& vector) const
	{
		glUniform4fv(glGetUniformLocation(m_id, name), 1, glm::value_ptr(vector));
	}

	void ShaderProgram::setFloat(const char* name, const float value) const {
		glUniform1f(glGetUniformLocation(m_id, name), value);
	}

	void ShaderProgram::setInt(const char* name, const int value) const {
		glUniform1i(glGetUniformLocation(m_id, name), value);
	}

	void ShaderProgram::setObject(
		const char* name, 
		std::map<char*, std::pair<PropertyTypes, std::any>> properties
	) const {
		for (auto& item : properties)
		{
			std::string propertyName = std::string(item.first);
			std::string objectPropertyFullName = std::string(name) + "." + propertyName;
		
			PropertyTypes propertyType = item.second.first;
			auto propertyValue = item.second.second;

			switch (item.second.first) {
				case PropertyTypes::Int: {
					setInt(objectPropertyFullName.c_str(), std::any_cast<int>(propertyValue));
					break;
				}
				case PropertyTypes::Float: {
					setFloat(objectPropertyFullName.c_str(), std::any_cast<float> (propertyValue));
					break;
				}
				case PropertyTypes::Vec3: {
					setVec3(objectPropertyFullName.c_str(), std::any_cast<glm::vec3>(propertyValue));
					break;
				}
				case PropertyTypes::Vec4: {
					setVec4(objectPropertyFullName.c_str(), std::any_cast<glm::vec4>(propertyValue));
					break;
				}
				case PropertyTypes::Mat3: {
					setMatrix3(objectPropertyFullName.c_str(), std::any_cast<glm::mat3>(propertyValue));
					break;
				}
				case PropertyTypes::Mat4: {
					setMatrix4(objectPropertyFullName.c_str(), std::any_cast<glm::mat4>(propertyValue));
					break;
				}
			}
		}
	}


	void ShaderProgram::setObjects(
		const char* name, 
		std::vector<std::map<char*, std::pair<PropertyTypes, std::any>>> objects
	) const {
		for (auto it = objects.begin(); it != objects.end(); ++it) {
			int index = std::distance(objects.begin(), it);
			std::string objectName = std::string(name) + "[" + std::to_string(index) + "]";
			setObject(objectName.c_str(), *it);
		}
	}

	void ShaderProgram::bind() const
	{
		glUseProgram(m_id);
	}

	void ShaderProgram::unbind()
	{
		glUseProgram(0);
	}

	unsigned int ShaderProgram::get() {
		return m_id;
	}

	//move
	ShaderProgram& ShaderProgram::operator=(ShaderProgram&& shaderProgram)
	{
		glDeleteProgram(m_id);
		m_id = shaderProgram.m_id;
		m_isCompiled = shaderProgram.m_isCompiled;

		shaderProgram.m_id = 0;
		shaderProgram.m_isCompiled = false;
		return *this;
	}

	ShaderProgram::ShaderProgram(ShaderProgram&& shaderProgram)
	{
		m_id = shaderProgram.m_id;
		m_isCompiled = shaderProgram.m_isCompiled;

		shaderProgram.m_id = 0;
		shaderProgram.m_isCompiled = false;
	}

}