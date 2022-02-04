#pragma once

#pragma once

#include "VertexBuffer.hpp"

namespace GameEngine {
	class VertexArray {
	public:

		VertexArray();
		~VertexArray();

		VertexArray(const VertexArray&) = delete;
		VertexArray& operator=(const VertexArray&) = delete;
		VertexArray(VertexArray&&) noexcept;
		VertexArray& operator=(VertexArray&&) noexcept;

		void addBuffer(const VertexBuffer& vertexBuffer);
		void bind() const;
		static void unbind();
	private:
		unsigned int m_id = 0;
		unsigned int m_elementsCount = 0;
	};
}