#version 330 core

in vec2 tCoord;

const vec3 skycolor = vec3(0.86f, 0.82f, 0.78f);
const float maxDist = 10.0f;

out vec4 fragcol;

vec3 applyFog(vec3 originalColor, float distance) {
    float fogDensity = 0.1; // Adjust this value to increase or decrease fog intensity
    float fogFactor = exp(-fogDensity * distance);
    
    vec3 fogColor = skycolor; // The color of the fog
    return mix(originalColor, fogColor, 1.0 - fogFactor);
}

in float dist;
in vec3 col;

void main(){
    vec3 tCol = vec3(0.0f);
    float d = min(dist, maxDist);
    float alpha = d/maxDist;
    // vec3 tempcol = (1-alpha)*tCol + (alpha)*skycolor;
    vec3 tempcol = applyFog(tCol, d);

    fragcol = vec4(tempcol, 1.0f);
    

    // fragcol = vec4(0.2f, 0.3f, 0.3f, 1.0f);
}