#version 460
layout(location = 0) in vec3 vertexPosition;

uniform mat4 transformMatrix;
uniform mat4 viewAndProjectionMatrix;

void main() {

    gl_Position = viewAndProjectionMatrix * transformMatrix * vec4(vertexPosition, 1.0);
}