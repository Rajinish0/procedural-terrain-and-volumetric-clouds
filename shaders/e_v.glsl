#version 330 core

layout (location =0) in vec3 aPos;
layout (location =1) in vec3 normal;
layout (location =2) in vec2 texCoord;

// uniform float[10000] zCoords;

// uniform sampler1D zCoordsTexture;


uniform int numVerts;
uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;
uniform vec4 planeNorm;

uniform vec2 offset;
uniform sampler2D heightMap;

out vec3 col;
out vec2 tCoord;
out float dist;

void main(){

    vec2 size = textureSize(heightMap, 0).xy;
    vec2 heightTexCoord = (vec2(aPos.x, -aPos.z) + vec2( float(size)/2.0f,  float(size)/2.0f)) / size;

    float y = texture(heightMap, texCoord).r * 15.0f;

    if (y < 0.0) {
        col = vec3(0.0, 0.0, 0.5); // Deep water (Dark Blue)
    } else if (y < 0.1) {
        col = vec3(0.0, 0.5, 1.0); // Shallow water (Light Blue)
    } else if (y < 0.3) {
        col = vec3(1.0, 0.9, 0.6); // Beach/sand (Sandy Beige)
    } else if (y < 0.5) {
        col = vec3(0.5, 1.0, 0.5); // Grass (Bright Green)
    } else if (y < 0.7) {
        col = vec3(0.0, 0.4, 0.0); // Grass with some depth (Dark Green)
    } else if (y < 0.9) {
        col = vec3(0.6, 0.5, 0.4); // Rock/Mountain (Rocky Gray)
    } else {
        col = vec3(1.0, 1.0, 1.0); // Snow (White)
    }

    vec4 worldPos = model*vec4(aPos.x, y, aPos.z, 1.0);
    vec4 viewPos = view * worldPos;
    dist = length(viewPos.xyz);
    gl_Position = proj*viewPos;
    // gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);
    // tCoord = texCoord;
}