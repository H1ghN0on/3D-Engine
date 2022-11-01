#include "Mesh.hpp"
#include "GameEngineCore/Log.hpp"

#include <iostream>


namespace GameEngine {
    Mesh::Mesh(BufferLayout _layout, std::vector<Vertex> _vertices, std::vector<unsigned int> _indices, std::vector<Texture> _textures)
        : vertices(_vertices)
        , indices(_indices)
        , textures(_textures)
        , layout(_layout)
    {
        setupMesh();
    }


    void Mesh::setupMesh() {
        vertexArray = std::make_shared<VertexArray>();
        vertexBuffer = std::make_unique<VertexBuffer>(vertices, layout);
        vertexArray->addVertexBuffer(*vertexBuffer);
        if (indices.size() != 0) {
            indexBuffer = std::make_unique<IndexBuffer>(std::data(indices), indices.size());
            vertexArray->setIndexBuffer(*indexBuffer);
        }
    }

    void Mesh::draw(std::shared_ptr<ShaderProgram> shader) {
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
 
        for (unsigned int i = 0; i < textures.size(); i++)
        {
            std::string textureName = "";
            glActiveTexture(GL_TEXTURE0 + i); // активируем текстурный блок, до привязки

            switch (textures[i].getType()) {
            case Texture::Type::Diffusal: {
                textureName = "texture_diffuse" + std::to_string(diffuseNr);
                diffuseNr++;

                break;
            }
            case Texture::Type::Specular: {
                textureName = "texture_specular" + std::to_string(specularNr);
                specularNr++;
                break;
            }
        }

            shader -> setFloat(("material." + textureName).c_str(), i);
            glBindTexture(GL_TEXTURE_2D, textures[i].getId());

            glActiveTexture(GL_TEXTURE0);
        }
        vertexArray->bind();
        Renderer::draw(*vertexArray);

    }
}