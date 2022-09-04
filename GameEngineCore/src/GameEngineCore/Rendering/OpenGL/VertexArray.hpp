#pragma once

#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"

namespace GameEngine {
	class VertexArray {
	public:

		VertexArray();
		~VertexArray();

		VertexArray(const VertexArray&) noexcept;
		VertexArray& operator=(const VertexArray&) noexcept;
		VertexArray(VertexArray&&) noexcept;
		VertexArray& operator=(VertexArray&&) noexcept;

		void addVertexBuffer(const VertexBuffer& vertexBuffer);
		void setIndexBuffer(const IndexBuffer& indexBuffer);
		void bind() const;
		bool hasIndexBuffer() const;
		static void unbind();
		size_t getIndicesCount() const { return m_indicesCount; }
		unsigned int getVerticesCount() const { return m_verticesCount; }
	private:
		unsigned int m_id = 0;
		unsigned int m_elementsCount = 0;
		unsigned int m_verticesCount = 0;
		size_t m_indicesCount = 0;
	};
}