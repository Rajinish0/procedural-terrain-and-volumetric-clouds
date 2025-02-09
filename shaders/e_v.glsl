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

uniform float maxHeight;

out vec3 col;
out vec2 tCoord;
out vec3 fragpos;

out vec3 normal;
out float dist;

void main(){

    /*
    TO DO:
        I think the normal calculations are good, only the edges are problematic
        so if aPos.x is on the edge instead of using the vertex with coord (aPos.x + 1, aPos.z)
        I should use (aPos.x - 1, aPos.z) (need to test this), if this creates a normal 
        in the opposite direction then the only way is to create a pseudo heightMap which has 
        2 extra rows and columns and i use this to calculate the height at the edges, this 
        should also solve the problem of there being a line b/w two chunks due to incorrect lighting
    */

    // vec2 size = textureSize(heightMap, 0).xy;
    // // bool xFlipped = ((gl_VertexID + 1) % 241 == 0);//> 0 && (gl_VertexID % 240 == 0) );
    // bool xFlipped = false;

    // vec2 heightTexCoord = (vec2(aPos.x, -aPos.z) + vec2( float(size.x)/2.0f,  float(size.y)/2.0f)) / size;
    // vec2 heightTexCoord2 = (vec2(aPos.x + 1, -aPos.z) + vec2( float(size.x)/2.0f,  float(size.y)/2.0f)) / size;
    // vec2 heightTexCoord3 = (vec2(aPos.x, -(aPos.z + 1)) + vec2( float(size.x)/2.0f,  float(size.y)/2.0f)) / size;

    vec4 info = texture(heightMap, texCoord );
    float y = info.r * maxHeight;
    // float y2 = texture(heightMap, heightTexCoord2 ).r * maxHeight;
    // float y3 = texture(heightMap, heightTexCoord3 ).r * maxHeight;

    // vec3 v1 = vec3(aPos.x, y, aPos.z);
    // vec3 v2 = vec3(aPos.x + (1), y2, aPos.z);
    // vec3 v3 = vec3(aPos.x, y3, aPos.z + 1);

    // vec3 cp = normalize(cross(v3 - v1, v2 - v1));
    // cp = cp / length(cp);
    // vec3 cp = texture(normalMap, texCoord).rgb;

    vec3 cp = info.gba * 2.0 - 1.0;

    normal = normalize(mat3(transpose(inverse(model))) * cp);
    if (y < 0.0*65.0) {
        col = vec3(0.0, 0.0, 0.5); // Deep water (Dark Blue)
    } else if (y < 0.1*65.0) {
        col = vec3(0.0, 0.5, 1.0); // Shallow water (Light Blue)
    } else if (y < 0.3*65.0) {
        col = vec3(1.0, 0.9, 0.6); // Beach/sand (Sandy Beige)
    } else if (y < 0.5*65.0) {
        col = vec3(0.5, 1.0, 0.5); // Grass (Bright Green)
    } else if (y < 0.7*65.0) {
        col = vec3(0.0, 0.4, 0.0); // Grass with some depth (Dark Green)
    } else if (y < 0.9*65.0) {
        col = vec3(0.6, 0.5, 0.4); // Rock/Mountain (Rocky Gray)
    } else {
        col = vec3(1.0, 1.0, 1.0); // Snow (White)
    }

    // if (xFlipped)
    //     col = vec3(0.0f, 0.0f, 0.0f);

    vec4 worldPos = model*vec4(aPos.x, y, aPos.z, 1.0);
    gl_ClipDistance[0] = dot(worldPos, planeNorm);
    fragpos = worldPos.xyz;
    vec4 viewPos = view * worldPos;
    dist = length(viewPos.xyz);
    gl_Position = proj*viewPos;
    // gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);
    tCoord = texCoord;
}
