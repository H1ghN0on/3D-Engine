#include "VertexBuffer.hpp"
#include "GameEngineCore/Log.hpp"

#include <glad/glad.h>

namespace GameEngine {
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

	VertexBuffer::VertexBuffer(const void* data, const size_t size, const EUsage usage) {
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

	VertexBuffer::VertexBuffer(VertexBuffer&& vertexBuffer) noexcept {
		m_id = vertexBuffer.m_id;
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