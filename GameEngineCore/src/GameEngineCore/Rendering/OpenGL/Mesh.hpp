#pragma once

#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include <vector>
#include <memory>
#include "glad/glad.h"
#include <string>
#include "Renderer.hpp"
#include "ShaderProgram.hpp";
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include <assimp/types.h>

namespace GameEngine {

    enum class TextureType {
        Diffusal, Specular,
    };

    //struct ObjectVertex {
    //    glm::vec3 Position;
    //    glm::vec3 Normal;
    //    glm::vec2 TexCoords;
    //};
    struct ObjectTexture {
        unsigned int id;
        TextureType type;
        aiString path;
    };

	class Mesh {
    public:
        std::vector<GLfloat> vertices;
        std::vector<unsigned int> indices;
        std::vector<ObjectTexture> textures;

        Mesh(BufferLayout layout, std::vector<GLfloat> vertices, std::vector<unsigned int> indices, std::vector<ObjectTexture> textures);
        void draw(ShaderProgram shader);
    private:
        BufferLayout layout;
        std::shared_ptr<VertexArray> vertexArray = nullptr;
        std::unique_ptr<VertexBuffer> vertexBuffer = nullptr;
        std::unique_ptr<IndexBuffer> indexBuffer = nullptr;
 
        void setupMesh();
	};
}