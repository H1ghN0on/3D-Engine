#version 460
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 normal;

uniform mat4 transformMatrix;
uniform mat4 viewAndProjectionMatrix;

out vec3 Normal;
out vec3 FragPos;
void main() {
    Normal = normal;
    FragPos = vec3(transformMatrix * vec4(vertexPosition, 1.0));
    gl_Position = viewAndProjectionMatrix * transformMatrix * vec4(vertexPosition, 1.0);
}