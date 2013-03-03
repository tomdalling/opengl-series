#version 150

in vec3 vert;

void main() {
    // does not alter the verticies at all
    gl_Position = vec4(vert, 1);
}