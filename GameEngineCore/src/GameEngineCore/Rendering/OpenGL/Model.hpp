#pragma once

#include "ShaderProgram.hpp"

#include <vector>
#include "Mesh.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Texture.hpp"


namespace GameEngine {
    class Model
    {
    public:
        /*  Методы   */
        Model(char* path)
        {
            loadModel(path);
        }
        void draw(ShaderProgram shader);
    private:
        /*  Данные модели  */
        std::vector<Mesh> meshes;
        std::string directory;
        /*  Методы   */
        void loadModel(std::string path);
        void processNode(aiNode* node, const aiScene* scene);
        Mesh processMesh(aiMesh* mesh, const aiScene* scene);
        std::vector<ObjectTexture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType typeName);
        std::vector<ObjectTexture> textures_loaded;
    };
}