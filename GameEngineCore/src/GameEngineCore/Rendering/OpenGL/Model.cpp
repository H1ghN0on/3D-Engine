#include "Model.hpp"
#include "GameEngineCore/Log.hpp";

#include <iostream>
#include "AssimpGLMHelpers.hpp"




namespace GameEngine {



    void Model::draw(std::shared_ptr<ShaderProgram> shader, bool points)
    {
    
        for (unsigned int i = 0; i < meshes.size(); i++)
            meshes[i].draw(shader, points);
    }



    Model::Model(
        std::vector<Vertex> vertices,
        std::vector<unsigned int> indices,
        std::vector<Texture> textures
    ) {
       
        meshes.push_back(
            Mesh(
                vertices,
                indices,
                textures
            )
        );
    }

    void Model::loadModel(std::string path) {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            LOG_CRITICAL("ERROR::ASSIMP::{0}", importer.GetErrorString());
            return;
        }
        directory = path.substr(0, path.find_last_of('/'));

        processNode(scene->mRootNode, scene);
    }


    void Model::processNode(aiNode* node, const aiScene* scene)
    {
        // обработать все полигональные сетки в узле(если есть)
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }
        // выполнить ту же обработку и для каждого потомка узла
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }
    }


    Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;



        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            if (scene->mAnimations) {
                std::cout << scene->mAnimations[0]->mName.C_Str() << std::endl;
            }
            
            glm::vec3 position;
            glm::vec3 normal(0.0f, 0.0f, 0.0f);
            glm::vec2 texture;

            position = AssimpGLMHelpers::GetGLMVec(mesh->mVertices[i]);
            if (mesh->HasNormals()) {
                normal = AssimpGLMHelpers::GetGLMVec(mesh->mNormals[i]);
            } 
            if (mesh->mTextureCoords[0]) { 
                texture.x = mesh->mTextureCoords[0][i].x;
                texture.y = mesh->mTextureCoords[0][i].y;
            } else {
                texture = glm::vec2(0.f, 0.f);
            }
            glm::vec3 color = { 1.0f, 0.0f, 0.0f };
            Vertex vertex(position, normal, texture, color);
            vertices.push_back(vertex);
        }


        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++) {
                indices.push_back(face.mIndices[j]);
            }
                
        }

        if (mesh->mMaterialIndex >= 0)
        {
     
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
            std::vector<Texture> diffuseMaps = loadMaterialTextures(material,
                aiTextureType_DIFFUSE, Texture::Type::Diffusal);
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
            std::vector<Texture> specularMaps = loadMaterialTextures(material,
                aiTextureType_SPECULAR, Texture::Type::Specular);
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        }

        return Mesh(
            vertices, 
            indices,
            textures
        );
    }

    std::vector<Texture> Model::loadMaterialTextures(aiMaterial * mat, aiTextureType type, Texture::Type typeName)
    {
        std::vector<Texture> textures;
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            bool skip = false;
            for (unsigned int j = 0; j < textures_loaded.size(); j++)
            {
                if (std::strcmp(textures_loaded[j].getLocation().c_str(), str.C_Str()) == 0)
                {
                    textures.push_back(textures_loaded[j]);
                    skip = true;
                    break;
                }
            }
            if (!skip)
            {   // если текстура не была загружена – сделаем это
                
                Texture texture(
                    std::string(directory + '/' + str.C_Str()).c_str(),
                    typeName, 
                    Texture::WrappingMode::Repeat,
                    Texture::MipmapFilterMode::LinearLinear
                );
       
                textures.push_back(texture);
                // занесем текстуру в список уже загруженных
                textures_loaded.push_back(texture);
            }
        }
        return textures;
    }
}