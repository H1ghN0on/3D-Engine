#pragma once

#include "ShaderProgram.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"


#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>



namespace GameEngine {
    class Model
    {
    public:
        /*  ������   */
        Model(const char* path) {
            loadModel(path);
        }

        Model(
            BufferLayout layout,
            float* vertices,
            size_t verticesSize,
            unsigned int* indices,
            unsigned int indicesCount
        );

        void draw(ShaderProgram shader);
    private:
        /*  ������ ������  */
        std::vector<Mesh> meshes;
        std::string directory;
        /*  ������   */
        void loadModel(std::string path);
        void processNode(aiNode* node, const aiScene* scene);
        Mesh processMesh(aiMesh* mesh, const aiScene* scene);
        std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, Texture::Type typeName);
        std::vector<Texture> textures_loaded;
    };
}