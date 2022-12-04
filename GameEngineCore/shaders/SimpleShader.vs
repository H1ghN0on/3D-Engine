#version 460
layout(location = 0) in vec3 vertexPosition;
layout(location = 3) in vec3 color;
uniform mat4 transformMatrix;
uniform mat4 viewAndProjectionMatrix;

out vec3 FragColor;

void main() {
    FragColor = color;
    gl_Position = viewAndProjectionMatrix * transformMatrix * vec4(vertexPosition, 1.0);
}