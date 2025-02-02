#version 430 core

layout (location =0) in vec2 aPos;
layout (location =1) in vec2 texCoord;


out vec2 tCoord;


uniform float xOff;
uniform float yOff;

void main(){
    gl_Position = vec4(aPos.x*0.2f + xOff , aPos.y*0.2 + yOff , 0.0f, 1.0f);
    tCoord = texCoord;
}


