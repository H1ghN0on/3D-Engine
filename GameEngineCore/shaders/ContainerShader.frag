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
};

uniform Light light;
uniform Material material;
uniform vec3 objectColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;
uniform vec3 lightPos;
uniform vec3 viewPos;
void main() {

        //УЧЕСТЬ МАТРИЦУ НОРМАЛЕЙ
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    vec3 emissionFactor = step(vec3(1.0), vec3(1.0) - vec3(texture(material.specular, TexCoords)));
    vec3 emission = emissionFactor * vec3(texture(material.emission, TexCoords));

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    //степень задаёт силу блеска
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

    color = vec4((emission + ambient + diffuse + specular) * objectColor, 1.0f);

}