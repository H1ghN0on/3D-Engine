#version 460

in vec3 FragColor;
out vec4 color;

void main() {
    color = vec4(FragColor, 1.f);
}