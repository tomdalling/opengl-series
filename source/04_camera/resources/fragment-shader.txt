#version 150

uniform sampler2D tex;

in vec2 fragTexCoord;

out vec4 finalColor;

void main() {
    //note: the texture function was called texture2D in older versions of GLSL
    finalColor = texture(tex, fragTexCoord);
}