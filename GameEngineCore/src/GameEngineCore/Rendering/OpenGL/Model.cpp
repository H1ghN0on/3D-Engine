#include "Model.hpp"
#include "GameEngineCore/Log.hpp";
#include "SOIL/SOIL.h"

namespace GameEngine {
    void Model::draw(ShaderProgram shader)
    {
        for (unsigned int i = 0; i < meshes.size(); i++)
            meshes[i].draw(shader);
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
        std::vector<GLfloat> vertices;
        std::vector<unsigned int> indices;
        std::vector<ObjectTexture> textures;

        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            vertices.push_back(mesh->mVertices[i].x);
            vertices.push_back(mesh->mVertices[i].y);
            vertices.push_back(mesh->mVertices[i].z);
            if (mesh->HasNormals()) {
  
                vertices.push_back(mesh->mNormals[i].x);
                vertices.push_back(mesh->mNormals[i].y);
                vertices.push_back(mesh->mNormals[i].z);
            } 
 
            if (mesh->mTextureCoords[0]) { 
       
                vertices.push_back(mesh->mTextureCoords[0][i].x);
                vertices.push_back(mesh->mTextureCoords[0][i].y);



            } else {
                vertices.push_back(0.0f);
                vertices.push_back(0.0f);
            }
        }
        // орбаботка индексов
    
            // обработка материала
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
            std::vector<ObjectTexture> diffuseMaps = loadMaterialTextures(material,
                aiTextureType_DIFFUSE, TextureType::Diffusal);
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
            std::vector<ObjectTexture> specularMaps = loadMaterialTextures(material,
                aiTextureType_SPECULAR, TextureType::Specular);
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        }

        return Mesh(
            BufferLayout{ 
                ShaderDataType::Float3,
                ShaderDataType::Float3,
                ShaderDataType::Float2
            },
            vertices, 
            indices,
            textures
        );
    }


    unsigned int TextureFromFile(const char* path, const std::string& directory)
    {
        std::string filename = std::string(path);
        filename = directory + '/' + filename;

        unsigned int textureID;
        glGenTextures(1, &textureID);

        int width, height, nrComponents;
        unsigned char* data = SOIL_load_image(filename.c_str(), &width, &height, &nrComponents, 0);
        if (data)
        {

            GLenum format;
            if (nrComponents == 1)
                format = GL_RED;
            else if (nrComponents == 3)
                format = GL_RGB;
            else if (nrComponents == 4)
                format = GL_RGBA;

            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            SOIL_free_image_data(data);
        }
        else
        {
            LOG_CRITICAL("Image load failed: {0}, {1}", filename, SOIL_last_result());
            SOIL_free_image_data(data);
        }

        return textureID;
    }


    std::vector<ObjectTexture> Model::loadMaterialTextures(aiMaterial * mat, aiTextureType type, TextureType typeName)
    {
        std::vector<ObjectTexture> textures;
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            bool skip = false;
            for (unsigned int j = 0; j < textures_loaded.size(); j++)
            {
                if (std::strcmp(textures_loaded[j].path.C_Str(), str.C_Str()) == 0)
                {
                    textures.push_back(textures_loaded[j]);
                    skip = true;
                    break;
                }
            }
            if (!skip)
            {   // если текстура не была загружена – сделаем это
                ObjectTexture texture;
                texture.id = TextureFromFile(str.C_Str(), directory);
                texture.type = typeName;
                texture.path = str;
                textures.push_back(texture);
                // занесем текстуру в список уже загруженных
                textures_loaded.push_back(texture);
            }
        }
        return textures;
    }
}