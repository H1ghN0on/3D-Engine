#version 460

out vec4 color;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
    sampler2D emission;
};

struct Light {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

};

uniform Light light;
uniform Material material;
uniform vec3 objectColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;
uniform vec3 viewPos;
void main() {

        //УЧЕСТЬ МАТРИЦУ НОРМАЛЕЙ

    vec3 norm = normalize(Normal);

    vec3 lightDir = normalize(light.position - FragPos);

    //затухание
    // float lightToFragDistance = length(light.position - FragPos);
    // float attenuation = 1.0 / (light.constant + light.linear * lightToFragDistance +
    //     light.quadratic * (lightToFragDistance * lightToFragDistance));

    //projector
    //угол между фрагментом и источником света
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;

    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

        // vec3 emissionFactor = step(vec3(1.0), vec3(1.0) - vec3(texture(material.specular, TexCoords)));
        // vec3 emission = emissionFactor * vec3(texture(material.emission, TexCoords));
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

        //степень задаёт силу блеска
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    diffuse *= intensity;
    specular *= intensity;
        // ambient *= attenuation;
        // diffuse *= attenuation;
        // specular *= attenuation;

    color = vec4((ambient + diffuse + specular) * objectColor, 1.0f);

        //для эмиссии
        //color = vec4((emission + ambient + diffuse + specular) * objectColor, 1.0f);

    //для направленного источника света
    // vec3 lightDir = normalize(-light.direction);

}