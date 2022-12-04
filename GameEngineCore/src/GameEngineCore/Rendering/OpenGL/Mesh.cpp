#include "Mesh.hpp"
#include "GameEngineCore/Log.hpp"

#include <iostream>


namespace GameEngine {
    Mesh::Mesh(std::vector<Vertex> _vertices, std::vector<unsigned int> _indices, std::vector<Texture> _textures)
        : vertices(_vertices)
        , indices(_indices)
        , textures(_textures)
    {
        setupMesh();
    }


    void Mesh::setupMesh() {
        vertexArray = std::make_shared<VertexArray>();
        vertexBuffer = std::make_shared<VertexBuffer>(vertices);
        vertexArray->addVertexBuffer(*vertexBuffer);
        if (indices.size() != 0) {
            indexBuffer = std::make_shared<IndexBuffer>(std::data(indices), indices.size());
            vertexArray->setIndexBuffer(*indexBuffer);
        }
    }

    void Mesh::updateMesh() {

        vertexBuffer->updateBuffer(vertices);
    }

    void Mesh::draw(std::shared_ptr<ShaderProgram> shader, bool points) {
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
 
        for (unsigned int i = 0; i < textures.size(); i++)
        {

            std::string textureName = "";
            glActiveTexture(GL_TEXTURE0 + i); // ���������� ���������� ����, �� ��������

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

