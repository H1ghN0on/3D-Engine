#pragma once

#include <vector>

namespace GameEngine {

	enum class ShaderDataType {
		Float,
		Float2,
		Float3,
		Float4,
		Int,
		Int2,
		Int3,
		Int4
	};

	struct BufferElement {
		ShaderDataType type;
		uint32_t componentType; //GL_FLOAT or GL_INT
		size_t componentsCount; //f.e. FLOAT3 == 3
		size_t size; // in bytes (f.e. FLOAT3 == sizeof(float) * 3
		size_t offset; //offset from beginning (f.e. first FLOAT3 has offset 0, second FLOAT3 has offset 3)
		BufferElement(const ShaderDataType type);
	};

	//Generate the layout of array attributes
	class BufferLayout {
	public: 
		BufferLayout(std::initializer_list<BufferElement> elements)
			: m_elements(elements) 
		{
			size_t offset = 0;
			m_stride = 0;
			//The first element has offset 0, the second element has offset = the size of first element ...{offset}...
			//m_stride = full size of ine Buffer Element (FLOAT3 + FLOAT3 = 6)
			for (auto& element : m_elements) {
				element.offset = offset;
				offset += element.size;
				m_stride += element.size;
			}
		}

		const std::vector<BufferElement>& getElements() const { return m_elements; }
		size_t getStride() const { return m_stride; };
	private:
		//all buffer elements
		std::vector<BufferElement> m_elements;
		size_t m_stride = 0;
	};



	class VertexBuffer {
	public:

		enum class EUsage {
			Static,
			Dynamic,
			Stream
		};

		VertexBuffer(
			unsigned int verticesCount,
			const void* data, 
			size_t size, 
			BufferLayout bufferLayout, 
			const EUsage usage = VertexBuffer::EUsage::Static
		);
		~VertexBuffer();

		VertexBuffer(const VertexBuffer&) = delete;
		VertexBuffer& operator=(const VertexBuffer&) = delete;
		VertexBuffer(VertexBuffer&&) noexcept;
		VertexBuffer& operator=(VertexBuffer&&) noexcept;

		void bind() const;
		static void unbind();
		unsigned int getVerticesCount() const { return verticesCount;  }
		const BufferLayout& getLayout() const { return m_bufferLayout; }

	private:
		unsigned int m_id = 0;
		BufferLayout m_bufferLayout;
		unsigned int verticesCount;
	};
}