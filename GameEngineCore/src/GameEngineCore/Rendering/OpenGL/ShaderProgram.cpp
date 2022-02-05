#include "ShaderProgram.hpp";
#include "GameEngineCore/Log.hpp";

#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
 
namespace GameEngine {
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

	ShaderProgram::ShaderProgram(const char* vertexShaderSrc, const char* fragmentShaderSrc) {
		//creating vertex and fragment shaders
		GLuint vertexShaderId = 0;
		if (!createShader(vertexShaderSrc, GL_VERTEX_SHADER, vertexShaderId)) {
			LOG_CRITICAL("VERTEX SHADER: compile-time error!");
			glDeleteShader(vertexShaderId);
			return;
		}

		GLuint fragmentShaderId = 0;
		if (!createShader(fragmentShaderSrc, GL_FRAGMENT_SHADER, fragmentShaderId)) {
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
	void ShaderProgram::setMatrix4(const char* name, const glm::mat4& matrix) const
	{
		glUniformMatrix4fv(glGetUniformLocation(m_id, name), 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void ShaderProgram::bind() const
	{
		glUseProgram(m_id);
	}

	void ShaderProgram::unbind()
	{
		glUseProgram(0);
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