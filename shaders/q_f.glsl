#version 330 core

in vec2 tCoord;

out vec4 fragcol;

uniform sampler2D texture_diffuse;

void main(){
    fragcol = texture(texture_diffuse, tCoord);
}