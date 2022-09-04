#version 460

out vec4 color;

uniform vec3 objectColor;
uniform vec3 lightColor;

in vec3 Normal;
in vec3 FragPos;
uniform vec3 lightPos;
uniform vec3 viewPos;
void main() {

        //УЧЕСТЬ МАТРИЦУ НОРМАЛЕЙ

    float specularStrength = 0.5f;
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

        //степень задаёт силу блеска
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    color = vec4((ambient + diffuse + specular) * objectColor, 1.0f);

}