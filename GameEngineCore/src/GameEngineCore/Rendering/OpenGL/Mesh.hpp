#pragma once

#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include <vector>
#include <memory>
#include "glad/glad.h"
#include <string>
#include "Renderer.hpp"
#include "ShaderProgram.hpp";
#include "Texture.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "Vertex.hpp"

namespace GameEngine {
 

	class Mesh {
    public:
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        Mesh(BufferLayout layout, std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
        void draw(std::shared_ptr<ShaderProgram> shader);
    private:
        BufferLayout layout;
        std::shared_ptr<VertexArray> vertexArray = nullptr;
        std::unique_ptr<VertexBuffer> vertexBuffer = nullptr;
        std::unique_ptr<IndexBuffer> indexBuffer = nullptr;
 
        void setupMesh();
	};
}