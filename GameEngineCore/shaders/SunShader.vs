#version 460
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 texCoord;
layout(location = 3) in vec3 color;

uniform mat4 transformMatrix;
uniform mat4 viewAndProjectionMatrix;

out vec3 FragColor;
out vec3 TexCoord;
out vec3 FragCoord;

void main() {
    FragCoord = vertexPosition;
    TexCoord = texCoord;
    FragColor = vec3(0.9, 0.5, 0.2);
    gl_Position = viewAndProjectionMatrix * transformMatrix * vec4(vertexPosition, 1.0);
}
