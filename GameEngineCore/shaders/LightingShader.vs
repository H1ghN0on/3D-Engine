#version 460
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

uniform mat4 transformMatrix;
uniform mat4 viewAndProjectionMatrix;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

void main() {
    TexCoords = texCoords;
    Normal = mat3(transpose(inverse(transformMatrix))) * normal;
    FragPos = vec3(transformMatrix * vec4(vertexPosition, 1.0));
    gl_Position = viewAndProjectionMatrix * transformMatrix * vec4(vertexPosition, 1.0);
}