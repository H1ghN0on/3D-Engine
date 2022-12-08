#include <GameEngineCore/ShaderManager.hpp>
#include <glm/trigonometric.hpp>
#include <GameEngineCore/ObjectManager.hpp>
namespace GameEngine {
	std::map<ShaderType, std::shared_ptr<ShaderProgram>> ShaderManager::shaders = std::map<ShaderType, std::shared_ptr<ShaderProgram>>();

    unsigned int ShaderManager::init() {
        std::shared_ptr<ShaderProgram> terrainShader, simpleShader, lightingTextureShader, lightingShader;

        lightingShader = std::make_shared<ShaderProgram>("LightingShader.vs", "LightingShader.frag");
        if (!lightingShader->isCompiled()) return 1;

        lightingTextureShader = std::make_shared<ShaderProgram>("LightingTextureShader.vs", "LightingTextureShader.frag");
        if (!lightingTextureShader->isCompiled()) return 1;

        terrainShader = std::make_shared<ShaderProgram>("TerrainShader.vs", "TerrainShader.frag");
        if (!terrainShader->isCompiled()) return 1;

        simpleShader = std::make_shared<ShaderProgram>("SimpleShader.vs", "SimpleShader.frag");
        if (!simpleShader->isCompiled()) return 1;
            
        shaders.insert(std::make_pair(ShaderType::LIGHTING, lightingShader));
        shaders.insert(std::make_pair(ShaderType::TERRAIN, terrainShader));
        shaders.insert(std::make_pair(ShaderType::SIMPLE, simpleShader));
        shaders.insert(std::make_pair(ShaderType::LIGHTING_TEXTURE, lightingTextureShader));

        return 0;
    }

    std::shared_ptr<ShaderProgram> ShaderManager::get(ShaderType type) {
        return shaders[type];
    }


    void ShaderManager::setMaterial(ShaderMaterial material) {
        ShaderManager::get(ShaderType::LIGHTING)->bind();
        switch (material) {
            case ShaderMaterial::EMERALD: {
                ShaderManager::get(ShaderType::LIGHTING)->setVec3("material.diffuse", glm::vec3(0.07568, 0.61424, 0.07568));
                ShaderManager::get(ShaderType::LIGHTING)->setVec3("material.specular", glm::vec3(0.633, 0.727811, 0.633));
                ShaderManager::get(ShaderType::LIGHTING)->setFloat("material.shininess", 0.6f * 128.f);
                break;
            }
            case ShaderMaterial::GOLD: {
                ShaderManager::get(ShaderType::LIGHTING)->setVec3("material.diffuse", glm::vec3(0.75164, 0.60648, 0.22648));
                ShaderManager::get(ShaderType::LIGHTING)->setVec3("material.specular", glm::vec3(0.628281, 0.555802, 0.366065));
                ShaderManager::get(ShaderType::LIGHTING)->setFloat("material.shininess", 0.4f * 128.f);
                break;
            }
            case ShaderMaterial::JADE: {
                ShaderManager::get(ShaderType::LIGHTING)->setVec3("material.diffuse", glm::vec3(0.54, 0.89, 0.63));
                ShaderManager::get(ShaderType::LIGHTING)->setVec3("material.specular", glm::vec3(0.316228, 0.316228, 0.1));
                ShaderManager::get(ShaderType::LIGHTING)->setFloat("material.shininess", 0.6f * 128.f);
                break;
            }
            case ShaderMaterial::PEARL: {
                ShaderManager::get(ShaderType::LIGHTING)->setVec3("material.diffuse", glm::vec3(1, 0.829, 0.829));
                ShaderManager::get(ShaderType::LIGHTING)->setVec3("material.specular", glm::vec3(0.296648, 0.296648, 0.296648));
                ShaderManager::get(ShaderType::LIGHTING)->setFloat("material.shininess", 0.088f * 128.f);
                break;
            }
            case ShaderMaterial::RUBBER: {
                ShaderManager::get(ShaderType::LIGHTING)->setVec3("material.diffuse", glm::vec3(0.07568, 0.61424, 0.07568));
                ShaderManager::get(ShaderType::LIGHTING)->setVec3("material.specular", glm::vec3(0.633, 0.727811, 0.633));
                ShaderManager::get(ShaderType::LIGHTING)->setFloat("material.shininess", 0.6f * 128.f);
                break;
            }

            case ShaderMaterial::DEFAULT: {
                ShaderManager::get(ShaderType::LIGHTING)->setVec3("material.diffuse", glm::vec3(1.f, 1.f, 1.f));
                ShaderManager::get(ShaderType::LIGHTING)->setVec3("material.specular", glm::vec3(1.f, 1.f, 1.f));
                ShaderManager::get(ShaderType::LIGHTING)->setFloat("material.shininess", 32.f);
                break;
            }
        }
    }

    void ShaderManager::setMaterial() {
        ShaderManager::get(ShaderType::LIGHTING_TEXTURE)->bind();
        ShaderManager::get(ShaderType::LIGHTING_TEXTURE)->setInt("material.diffuse", 0);
        ShaderManager::get(ShaderType::LIGHTING_TEXTURE)->setInt("material.specular", 1);
        ShaderManager::get(ShaderType::LIGHTING_TEXTURE)->setFloat("material.shininess", 32.f);
    }

    void ShaderManager::setLightInfo(
        std::vector<LightSource> dirLightSrcs,
        //glm::vec3 spotLightPosition,
        //glm::vec3 spotLightDirection,
        std::vector<LightSource> pointLightSrcs
    ) {
        

        if (dirLightSrcs.size() != 0) {

            shader_property dirLight = {
            { "ambient", { ShaderProgram::PropertyTypes::Vec3, glm::vec3(0.03f, 0.03f, 0.03f) } },
            { "diffuse", { ShaderProgram::PropertyTypes::Vec3, dirLightSrcs[0].diffuseColor}},
            { "specular", { ShaderProgram::PropertyTypes::Vec3, dirLightSrcs[0].specularColor}},
            { "direction", { ShaderProgram::PropertyTypes::Vec3, dirLightSrcs[0].direction}},
            };

            ShaderManager::get(ShaderType::LIGHTING_TEXTURE)->setObject("dirLight", dirLight);
            ShaderManager::get(ShaderType::LIGHTING)->setObject("dirLight", dirLight);
            ShaderManager::get(ShaderType::TERRAIN)->setObject("dirLight", dirLight);
        }

        //shader_property spotLight = {
        //    { "ambient", { ShaderProgram::PropertyTypes::Vec3, glm::vec3(0.05f, 0.05f, 0.05f) } },
        //    { "diffuse", { ShaderProgram::PropertyTypes::Vec3, glm::vec3(0.4f, 0.4f, 0.4f) } },
        //    { "specular", { ShaderProgram::PropertyTypes::Vec3, glm::vec3(0.5, 0.5f, 0.5f) } },
        //    { "cutOff", { ShaderProgram::PropertyTypes::Float, glm::cos(glm::radians(12.5f)) } },
        //    { "outerCutOff", { ShaderProgram::PropertyTypes::Float, glm::cos(glm::radians(17.5f)) } },
        //    { "position", { ShaderProgram::PropertyTypes::Vec3, spotLightPosition } },
        //    { "direction", { ShaderProgram::PropertyTypes::Vec3, spotLightDirection } },
        //    { "constant", { ShaderProgram::PropertyTypes::Float, 1.0f } },
        //    { "linear", { ShaderProgram::PropertyTypes::Float, 0.09f } },
        //    { "quadratic", { ShaderProgram::PropertyTypes::Float, 0.032f } },
        //};

        shader_property pointLight = {
            { "ambient", { ShaderProgram::PropertyTypes::Vec3, glm::vec3(0.03f, 0.03f, 0.03f) } },
            { "constant", { ShaderProgram::PropertyTypes::Float, 1.0f } },
            { "linear", { ShaderProgram::PropertyTypes::Float, 0.09f } },
            { "quadratic", { ShaderProgram::PropertyTypes::Float, 0.032f } },
        };

        std::vector<shader_property> pointLights;

        for (const auto pointLightSrc : pointLightSrcs) {
            pointLight["position"] = { ShaderProgram::PropertyTypes::Vec3, pointLightSrc.position };
            pointLight["diffuse"] = { ShaderProgram::PropertyTypes::Vec3, pointLightSrc.diffuseColor };
            pointLight["specular"] = { ShaderProgram::PropertyTypes::Vec3, pointLightSrc.specularColor };
            pointLights.push_back(pointLight);
            pointLight.erase("position");
            pointLight.erase("diffuse");
            pointLight.erase("specular");
        }


        ShaderManager::get(ShaderType::LIGHTING_TEXTURE)->bind();

        ShaderManager::get(ShaderType::LIGHTING_TEXTURE)->setVec3("viewPos", ObjectManager::getCamera()->getPosition());

        ShaderManager::get(ShaderType::LIGHTING_TEXTURE)->setInt("withFlashLight", false);
        ShaderManager::get(ShaderType::LIGHTING_TEXTURE)->setObjects("pointLights", pointLights);
        ShaderManager::get(ShaderType::LIGHTING_TEXTURE)->setInt("pointLightsNumber", pointLights.size());

        ShaderManager::get(ShaderType::LIGHTING_TEXTURE)->setInt("material.diffuse", 0);
        ShaderManager::get(ShaderType::LIGHTING_TEXTURE)->setInt("material.specular", 1);
        ShaderManager::get(ShaderType::LIGHTING_TEXTURE)->setFloat("material.shininess", 32.f);

       
        //ShaderManager::get(ShaderType::LIGHTING_TEXTURE)->setObject("spotLight", spotLight);


        ShaderManager::get(ShaderType::LIGHTING)->bind();
        ShaderManager::get(ShaderType::LIGHTING)->setVec3("viewPos", ObjectManager::getCamera()->getPosition());
        //material update
        
        //ShaderManager::get(ShaderType::LIGHTING)->setObject("spotLight", spotLight);
        ShaderManager::get(ShaderType::LIGHTING)->setInt("withFlashLight", false);
        ShaderManager::get(ShaderType::LIGHTING)->setObjects("pointLights", pointLights);
        ShaderManager::get(ShaderType::LIGHTING)->setInt("pointLightsNumber", pointLights.size());

        ShaderManager::get(ShaderType::TERRAIN)->bind();
        ShaderManager::get(ShaderType::TERRAIN)->setInt("material.diffuse", 0);
        ShaderManager::get(ShaderType::TERRAIN)->setInt("material.specular", 1);
        ShaderManager::get(ShaderType::TERRAIN)->setFloat("material.shininess", 32.f);
        
        //ShaderManager::get(ShaderType::TERRAIN)->setObject("spotLight", spotLight);
        ShaderManager::get(ShaderType::TERRAIN)->setInt("withFlashLight", false);
        ShaderManager::get(ShaderType::TERRAIN)->setObjects("pointLights", pointLights);
        ShaderManager::get(ShaderType::TERRAIN)->setInt("pointLightsNumber", pointLights.size());

        ShaderManager::get(ShaderType::TERRAIN)->setVec3("viewPos", ObjectManager::getCamera()->getPosition());

    }

    void ShaderManager::setViewAndProjectionMatrix(glm::mat4 mat) {
        for (auto& [key, shader] : shaders) {
            shader->bind();
            shader->setMatrix4("viewAndProjectionMatrix", mat);
        }
    }

       
}