#version 430 core

layout (location =0) in vec2 aPos;
layout (location =1) in vec2 texCoord;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

out vec4 finPos;
out vec4 worldPos;
out vec2 tCoord;
out float distFromCamera;

vec4 viewPos;


void main(){
    worldPos = model*vec4(aPos.x, aPos.y, 0.0, 1.0);
    viewPos = view * worldPos;
    distFromCamera = length(viewPos.xyz);
    finPos = proj*viewPos;
    gl_Position = finPos;
    tCoord = texCoord * 4.0f;
    // gl_Position = vec4(aPos.x*3.0f, aPos.y*3.0f, 0.0f, 1.0f);
}