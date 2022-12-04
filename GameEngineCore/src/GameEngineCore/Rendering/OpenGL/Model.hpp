#pragma once

#include "ShaderProgram.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"


#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <GameEngineCore/Vertex.hpp>


namespace GameEngine {

   

    class Model
    {
    public:

        /*  Методы   */
        Model(const char* path) {
            loadModel(path);
        }


        Model(
            std::vector<Vertex> vertices,
            std::vector<unsigned int> indices,
            std::vector<Texture> textures
        );

        void draw(std::shared_ptr<ShaderProgram> shader, bool points = false);

        std::vector<Mesh>& getMeshes() { return meshes;  }

    private:
        /*  Данные модели  */
        std::vector<Mesh> meshes;
        std::string directory;
        /*  Методы   */
        void loadModel(std::string path);
        void processNode(aiNode* node, const aiScene* scene);
        Mesh processMesh(aiMesh* mesh, const aiScene* scene);
        std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, Texture::Type typeName);
        std::vector<Texture> textures_loaded;



    };
}