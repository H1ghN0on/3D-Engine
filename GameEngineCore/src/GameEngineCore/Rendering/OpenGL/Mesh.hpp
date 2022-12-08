#pragma once

#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include <vector>
#include <memory>
#include "glad/glad.h"
#include <string>
#include <GameEngineCore/Renderer.hpp>
#include "ShaderProgram.hpp";
#include "Texture.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"


namespace GameEngine {
 

	class Mesh {
    public:
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        std::vector<Vertex>& getVertices() { return vertices; }

        void Mesh::updateMesh();
        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
   
        void updateColor(glm::vec3 color);

        void draw(std::shared_ptr<ShaderProgram> shader, bool points = false);
    private:
        std::shared_ptr<VertexArray> vertexArray = nullptr;
        std::shared_ptr<VertexBuffer> vertexBuffer = nullptr;
        std::shared_ptr<IndexBuffer> indexBuffer = nullptr;
 
        void setupMesh();
	};
}