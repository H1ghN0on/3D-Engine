#version 460
layout(location = 0) in vec3 vertexPosition;
layout(location = 3) in vec3 color;
uniform mat4 transformMatrix;
uniform mat4 viewAndProjectionMatrix;

out flat vec3 surfColor;

void main() {
    surfColor = color;
    gl_Position = viewAndProjectionMatrix * transformMatrix * vec4(vertexPosition, 1.0);
}