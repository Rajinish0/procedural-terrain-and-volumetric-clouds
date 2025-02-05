#version 430 core

layout (location =0) in vec3 aPos;
layout (location =1) in vec3 norma;
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
uniform sampler2D normalMap;

out vec3 col;
out vec2 tCoord;

out vec3 normal;
out float dist;

out VS_OUT{
	vec3 normal;
} vs_out;


void main(){

    // vec2 size = textureSize(heightMap, 0).xy;
    // vec2 heightTexCoord = (vec2(aPos.x, -aPos.z) + vec2( float(size)/2.0f,  float(size)/2.0f)) / size;
    // vec2 heightTexCoord2 = (vec2(aPos.x + 1, -aPos.z) + vec2( float(size)/2.0f,  float(size)/2.0f)) / size;
    // vec2 heightTexCoord3 = (vec2(aPos.x, -(aPos.z + 1)) + vec2( float(size)/2.0f,  float(size)/2.0f)) / size;

    // float y = texture(heightMap, texCoord).r * 65.0f;
    // float y2 = texture(heightMap, heightTexCoord2).r * 65.0f;
    // float y3 = texture(heightMap, heightTexCoord3).r * 65.0f;

    // vec3 v1 = vec3(aPos.x, y, aPos.z);
    // vec3 v2 = vec3(aPos.x + 1, y2, aPos.z);
    // vec3 v3 = vec3(aPos.x, y3, aPos.z + 1);

    vec4 info = texture(heightMap, texCoord);
    float y = info.r * 65.0f;
    // vec3 cp = cross(v3 - v1, v2 - v1);
    // cp = cp / length(cp);
    vec3 cp = info.gba * 2.0 - 1.0;

    vs_out.normal = normalize(mat3(transpose(inverse(view * model))) * cp);

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
    gl_Position = viewPos;
    // gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);
    // tCoord = texCoord;
}