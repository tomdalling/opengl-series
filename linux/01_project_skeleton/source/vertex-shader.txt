#version 150

in vec4 vert;

void main() {
    // does not alter the verticies at all
    gl_Position = vert;
}