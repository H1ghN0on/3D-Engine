#include "glad/glad.h"

#include "VertexArray.hpp"
#include "GameEngineCore/Log.hpp"

namespace GameEngine {
	//create vertex array buffer
	VertexArray::VertexArray() {
		glGenVertexArrays(1, &m_id);
	}

	//destroy vertex array buffer
	VertexArray::~VertexArray() {
		glDeleteVertexArrays(1, &m_id);
	}

	VertexArray& VertexArray::operator=(const VertexArray& vertexArray) noexcept {
		m_id = vertexArray.m_id;
		m_elementsCount = vertexArray.m_elementsCount;
		m_indicesCount = vertexArray.m_indicesCount;
		return *this;
	}

	VertexArray::VertexArray(const VertexArray& vertexArray) noexcept {
		m_id = vertexArray.m_id;
		m_elementsCount = vertexArray.m_elementsCount;
		m_indicesCount = vertexArray.m_indicesCount;
	}

	//move
	VertexArray& VertexArray::operator=(VertexArray&& vertexArray) noexcept {
		m_id = vertexArray.m_id;
		m_elementsCount = vertexArray.m_elementsCount;
		m_indicesCount = vertexArray.m_indicesCount;
		vertexArray.m_id = 0;
		vertexArray.m_elementsCount = 0;
		vertexArray.m_indicesCount = 0;
		return *this;
	}

	VertexArray::VertexArray(VertexArray&& vertexArray) noexcept
		: m_id(vertexArray.m_id)
		, m_elementsCount(vertexArray.m_elementsCount)
		, m_indicesCount(vertexArray.m_indicesCount)
	{
		vertexArray.m_id = 0;
		vertexArray.m_elementsCount = 0;
		vertexArray.m_indicesCount = 0;
	}

	//make the buffer current
	void VertexArray::bind() const {
		glBindVertexArray(m_id);
		
	}
	//make the buffer uncurrent
	void VertexArray::unbind() {
		glBindVertexArray(0);
	}

	void VertexArray::addVertexBuffer(const VertexBuffer& vertexBuffer) {
		//connecting vao and vbo
		bind();
		vertexBuffer.bind();
		m_verticesCount = vertexBuffer.getVerticesCount();
		//get the buffer layout, then setting buffer elements of layout to vao
		for (const BufferElement& current_element : vertexBuffer.getLayout().getElements())
		{
			glEnableVertexAttribArray(m_elementsCount);
			glVertexAttribPointer(
				m_elementsCount,
				static_cast<GLint>(current_element.componentsCount),
				current_element.componentType,
				GL_FALSE,
				static_cast<GLsizei>(vertexBuffer.getLayout().getStride()),
				reinterpret_cast<const void*>(current_element.offset)

			);
			
			++m_elementsCount;
		}
		
	}

	void VertexArray::setIndexBuffer(const IndexBuffer& indexBuffer) {
		//connecting vao and vbo
		bind();
		indexBuffer.bind();
		m_indicesCount = indexBuffer.getCount();
	}

	bool VertexArray::hasIndexBuffer() const {
		return m_indicesCount == 0;
	}
}
