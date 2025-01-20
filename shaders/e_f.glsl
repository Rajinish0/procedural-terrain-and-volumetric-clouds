#version 330 core

in vec2 tCoord;
in vec3 fragpos;
in vec3 normal;

const vec3 skycolor = vec3(0.86f, 0.82f, 0.78f);
// const vec3 lightPos = vec3(0.0f, 20.0f, 0.0f);
const float maxDist = 10.0f;
const vec3 sunDirection = normalize(vec3(0.0f, 1.0f, -2.0f));


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
    vec3 lightPos = fragpos + sunDirection;
    vec3 vecToLight = normalize(lightPos - fragpos);
    //vec3(0.7f, 0.9f, 0.8f)
    //vec3(0.58823, 0.43529, 0.2)
    vec3 tCol = vec3(0.60784, 0.43529, 0.31372)*max(dot(vecToLight, normal), 0.0f) + 
                vec3(0.1f);
    float d = min(dist, maxDist);
    float alpha = d/maxDist;
    // vec3 tempcol = (1-alpha)*tCol + (alpha)*skycolor;
    vec3 tempcol = applyFog(tCol, d);

    fragcol = vec4(tempcol, 1.0f);
    // fragcol = vec4(col, 1.0f);
    

    // fragcol = vec4(0.2f, 0.3f, 0.3f, 1.0f);
}
