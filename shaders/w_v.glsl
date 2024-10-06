#version 330 core

layout (location =0) in vec2 aPos;
layout (location =1) in vec2 texCoord;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

out vec4 finPos;

void main(){
    finPos = proj*view*model*vec4(aPos.x, aPos.y, 0.0, 1.0);
    gl_Position = finPos;
    //tCoord = texCoord;
    // gl_Position = vec4(aPos.x*3.0f, aPos.y*3.0f, 0.0f, 1.0f);
}