#include "Mesh.hpp"
#include "GameEngineCore/Log.hpp"
namespace GameEngine {
    Mesh::Mesh(BufferLayout _layout, std::vector<GLfloat> _vertices, std::vector<unsigned int> _indices, std::vector<ObjectTexture> _textures)
        : vertices(_vertices)
        , indices(_indices)
        , textures(_textures)
        , layout(_layout)
    {
        setupMesh();
    }


    void Mesh::setupMesh() {
        vertexArray = std::make_shared<VertexArray>();
        vertexBuffer = std::make_unique<VertexBuffer>(indices.size(), std::data(vertices), vertices.size() * sizeof(GLfloat), layout);
        vertexArray->addVertexBuffer(*vertexBuffer);
        if (indices.size() != 0) {
            indexBuffer = std::make_unique<IndexBuffer>(std::data(indices), indices.size());
            vertexArray->setIndexBuffer(*indexBuffer);
        }
    }

    void Mesh::draw(ShaderProgram shader) {
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;

        for (unsigned int i = 0; i < textures.size(); i++)
        {
            std::string textureName = "";
            glActiveTexture(GL_TEXTURE0 + i); // ���������� ���������� ����, �� ��������

            switch (textures[i].type) {
            case TextureType::Diffusal: {
                textureName = "texture_diffuse" + std::to_string(diffuseNr);
                diffuseNr++;

                break;
            }
            case TextureType::Specular: {
                textureName = "texture_specular" + std::to_string(specularNr);
                specularNr++;
                break;
            }
            }

            shader.setFloat(("material." + textureName).c_str(), i);
            glBindTexture(GL_TEXTURE_2D, textures[i].id);

            glActiveTexture(GL_TEXTURE0);
        }
            vertexArray->bind();
            Renderer::draw(*vertexArray);

    }
}