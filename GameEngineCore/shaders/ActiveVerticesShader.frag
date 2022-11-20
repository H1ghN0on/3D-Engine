#version 460
out vec4 color;

flat in vec3 surfColor;

void main() {
    color = vec4(surfColor, 1.f);
}