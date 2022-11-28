#version 460

out vec4 color;

struct Material {
    vec3 diffuse;
    vec3 specular;
    float shininess;
    vec3 emission;
};

uniform Material material;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 viewPos;

struct DirLight {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 direction;
};

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 direction;
    vec3 position;
    float cutOff;
    float outerCutOff;
    float constant;
    float linear;
    float quadratic;
};

#define MAX_LIGHTS 128

uniform bool withFlashlight;
uniform int pointLightsNumber;
uniform DirLight dirLight;
uniform PointLight pointLights[MAX_LIGHTS];
uniform SpotLight spotLight;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {

    //направление света от источника
    vec3 lightDir = normalize(-light.direction);

    // vec3 emissionFactor = step(vec3(1.0), vec3(1.0) - vec3(texture(material.specular, TexCoords)));
    // vec3 emission = emissionFactor * vec3(texture(material.emission, TexCoords));
    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // комбинируем результаты
    vec3 ambient = light.ambient * material.diffuse;
    vec3 diffuse = light.diffuse * diff * material.diffuse;
    vec3 specular = light.specular * spec * material.specular;
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 fragPos) {
    //затухание
    vec3 lightDir = normalize(light.position - fragPos);
    float lightToFragDistance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * lightToFragDistance +
        light.quadratic * (lightToFragDistance * lightToFragDistance));

    //расчёт света

    // vec3 emissionFactor = step(vec3(1.0), vec3(1.0) - vec3(texture(material.specular, TexCoords)));
    // vec3 emission = emissionFactor * vec3(texture(material.emission, TexCoords));

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 ambient = light.ambient * material.diffuse;
    vec3 diffuse = light.diffuse * diff * material.diffuse, TexCoords;
    vec3 specular = light.specular * spec * material.specular;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 fragPos) {
    vec3 lightDir = normalize(light.position - fragPos);
    //projector
    //угол между фрагментом и источником света
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    //интенсивность света между границами конусов
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    //затухание
    float lightToFragDistance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * lightToFragDistance + light.quadratic * (lightToFragDistance * lightToFragDistance));    

    //расчёт света

    // vec3 emissionFactor = step(vec3(1.0), vec3(1.0) - vec3(texture(material.specular, TexCoords)));
    // vec3 emission = emissionFactor * vec3(texture(material.emission, TexCoords));

    vec3 ambient = light.ambient * material.diffuse;
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * material.specular;

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 diffuse = light.diffuse * diff * material.diffuse;

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    return (ambient + diffuse + specular);
}

void main() {

    //УЧЕСТЬ МАТРИЦУ НОРМАЛЕЙ

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    // фаза 1: Направленный источник освещения
    vec3 result = CalcDirLight(dirLight, norm, viewDir);

    //фаза 2: Точечные источники
    for(int i = 0; i < pointLightsNumber; i++) result += CalcPointLight(pointLights[i], norm, viewDir, FragPos);    

    // фаза 3: фонарик
    if(withFlashlight)
        result += CalcSpotLight(spotLight, norm, viewDir, FragPos);

    color = vec4(result, 1.0f);

}