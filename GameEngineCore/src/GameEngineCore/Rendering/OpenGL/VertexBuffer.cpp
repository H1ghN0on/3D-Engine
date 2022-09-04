#include "VertexBuffer.hpp"
#include "GameEngineCore/Log.hpp"

#include <glad/glad.h>

namespace GameEngine {

    constexpr unsigned int shaderDataTypeToComponentsCount(const ShaderDataType type)
    {
        switch (type)
        {
        case ShaderDataType::Float:
        case ShaderDataType::Int:
            return 1;

        case ShaderDataType::Float2:
        case ShaderDataType::Int2:
            return 2;

        case ShaderDataType::Float3:
        case ShaderDataType::Int3:
            return 3;

        case ShaderDataType::Float4:
        case ShaderDataType::Int4:
            return 4;
        }

        LOG_ERROR("shaderDataTypeToComponentsCount: unknown ShaderDataType!");
        return 0;
    }


    constexpr size_t shaderDataTypeSize(const ShaderDataType type)
    {
        switch (type)
        {
        case ShaderDataType::Float:
        case ShaderDataType::Float2:
        case ShaderDataType::Float3:
        case ShaderDataType::Float4:
            return sizeof(GLfloat) * shaderDataTypeToComponentsCount(type);

        case ShaderDataType::Int:
        case ShaderDataType::Int2:
        case ShaderDataType::Int3:
        case ShaderDataType::Int4:
            return sizeof(GLint) * shaderDataTypeToComponentsCount(type);
        }

        LOG_ERROR("shaderDataTypeSize: unknown ShaderDataType!");
        return 0;
    }


    constexpr unsigned int shaderDataTypeToComponentType(const ShaderDataType type)
    {
        switch (type)
        {
        case ShaderDataType::Float:
        case ShaderDataType::Float2:
        case ShaderDataType::Float3:
        case ShaderDataType::Float4:
            return GL_FLOAT;
            
        case ShaderDataType::Int:
        case ShaderDataType::Int2:
        case ShaderDataType::Int3:
        case ShaderDataType::Int4:
            return GL_INT;
        }

        LOG_ERROR("shaderDataTypeToComponentType: unknown ShaderDataType!");
        return GL_FLOAT;
    }

	//cast EUsage to GLenum (user type to opengl type)
	constexpr GLenum usageToGLenum(const VertexBuffer::EUsage usage) {
		switch (usage) {
			//write data 1 time. use a lot of times
			case VertexBuffer::EUsage::Static: return GL_STATIC_DRAW;
			//write data a lot of times. use a lot of times
			case VertexBuffer::EUsage::Dynamic: return GL_DYNAMIC_DRAW;
			//write data 1 time. use 1 (or few) time
			case VertexBuffer::EUsage::Stream: return GL_STREAM_DRAW;
		}
		LOG_ERROR("Unknown VertexBuffer usage");
		return GL_STREAM_DRAW;
	}

    //Generate Buffer Element info from its name (f.e.FLOAT3 means array of 3 GL_Float elements with the 24bytes size)
	BufferElement::BufferElement(const ShaderDataType _type)
		: type(_type)
		, componentType(shaderDataTypeToComponentType(_type))
		, componentsCount(shaderDataTypeToComponentsCount(_type))
		, size(shaderDataTypeSize(_type))
		, offset(0)
	{
	}


	VertexBuffer::VertexBuffer(unsigned int verticesCount, const void* data, size_t size, BufferLayout bufferLayout, const EUsage usage) 
		: m_bufferLayout(std::move(bufferLayout)) 
        , verticesCount(verticesCount)
	{
		//generate new buffer
		glGenBuffers(1, &m_id);
		//make the buffer current
		glBindBuffer(GL_ARRAY_BUFFER, m_id);
		//write data to the current buffer (this one)
		glBufferData(GL_ARRAY_BUFFER, size, data, usageToGLenum(usage));
	}
	//destroy buffer
	VertexBuffer::~VertexBuffer() {
		glDeleteBuffers(1, &m_id);
	}
	//move buffer
	VertexBuffer& VertexBuffer::operator=(VertexBuffer&& vertexBuffer) noexcept {
		m_id = vertexBuffer.m_id;
		vertexBuffer.m_id = 0;
		return *this;
	}

	VertexBuffer::VertexBuffer(VertexBuffer&& vertexBuffer) noexcept 
        : m_id(vertexBuffer.m_id)
        , m_bufferLayout(std::move(vertexBuffer.m_bufferLayout))
    {
		vertexBuffer.m_id = 0;
	}
	//make the buffer current
	void VertexBuffer::bind() const {
		glBindBuffer(GL_ARRAY_BUFFER, m_id);
	}
	//make the buffer uncurrent
	void VertexBuffer::unbind()  {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

}