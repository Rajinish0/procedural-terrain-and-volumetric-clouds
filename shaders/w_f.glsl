#version 430 core

out vec4 fragcol;

in vec4 worldPos;
in vec4 finPos;
in vec2 tCoord;
in float distFromCamera;

const vec3 skycolor = vec3(0.86f, 0.82f, 0.78f);

uniform sampler2D dudv;
uniform sampler2D t1;
uniform sampler2D t2;
uniform sampler2D normalMap;
uniform sampler2D depthMap;
uniform float moveFac;
uniform vec3 planeNorm;
uniform vec3 camPos;
uniform vec3 sunDirection;

const float distortStrength = 0.05f;
const float maxDist = 20.0f;
float shineDamper = 30.0f;
float reflectivity = 0.6f;

vec3 applyFog(vec3 originalColor, float distance) {
    float fogAmt = 1.0 - exp(-distance*.0004);
    vec3 fogColor = vec3(0.529,0.708,0.922);//vec3(0.5, 0.6, 0.7);
    return mix(originalColor, fogColor, fogAmt);
}


void main(){

    //vec2 distort = (texture(dudv, vec2(tCoord.x + moveFac, tCoord.y)).rg*2 - 1) * distortStrength;
    //vec2 distort2 = (texture(dudv, vec2(-tCoord.x + moveFac, tCoord.y + moveFac)).rg*2 - 1) * distortStrength;

    vec2 ndc = (finPos.xy/finPos.w)/2.0f + 0.5f;
    vec2 refractTexCoords = vec2(ndc.x, ndc.y);
    vec2 reflectTexCoords = vec2(ndc.x, -ndc.y);

    float near = 0.1;
    float far = 500.0;
    float depth = texture(depthMap, refractTexCoords).r;
    float floorDist = 2.0 * near * far / (far + near - (2.0 * depth - 1.0) * (far - near));

    depth = gl_FragCoord.z;
    float waterDist = 2.0 * near * far / (far + near - (2.0 * depth - 1.0) * (far - near));

    float waterDepth = floorDist - waterDist;

    vec2 distortedCoord = texture(dudv, vec2(tCoord.x + moveFac, tCoord.y)).rg* 0.1;
    distortedCoord = tCoord + vec2(distortedCoord.x, distortedCoord.y + moveFac);
    //vec2 distortT = distort + distort2;
    vec2 distortT = (texture(dudv, distortedCoord).rg * 2.0 - 1.0) * distortStrength * clamp(waterDepth/20.0f, 0.0, 1.0);

    vec4 normalMapColour = texture(normalMap, distortedCoord);
    vec3 normal = vec3(normalMapColour.r * 2.0 - 1.0, 
                       normalMapColour.b * 3.0, 
                       normalMapColour.g * 2.0 - 1.0);
    normal = normalize(normal);

    vec3 diff = normalize(camPos - worldPos.xyz);
    float d = max(dot(normal, diff), 0.0f);

    
    vec2 tC = ((finPos/finPos.w).xy + 1.0f)/2.0f;
    vec2 reflect_c = vec2(tC.x, 1-tC.y);
    vec2 refrect_c = tC;

    reflect_c += distortT;
    refrect_c += distortT;

    reflect_c = clamp(reflect_c, 0.001, 0.999);
    refrect_c = clamp(refrect_c, 0.001, 0.999);
    float di = min(distFromCamera, maxDist);
    float alpha = di / maxDist;


    vec3 toCamera = normalize(camPos - worldPos.xyz);

    vec3 reflectedLight = reflect(-sunDirection, normal);
    float specular = max(dot(reflectedLight, toCamera), 0.0f);
    specular = pow(specular, shineDamper);
    vec3 specularHighlights = vec3(1.0f) * specular * reflectivity * clamp(waterDepth/5.0f, 0.0, 1.0);


    // fragcol = mix(texture(t1, reflect_c), texture(t2, refrect_c), d);
    // fragcol = mix(fragcol, vec4(0.0, 0.3, 0.5, 1.0), 0.2);
    vec3 tempcol = mix(texture(t1, reflect_c), texture(t2, refrect_c), d).xyz;
    // tempcol = mix(tempcol, vec3(0.0, 0.3, 0.5), 0.2);

    vec3 shallowColor = vec3(0.3, 0.5, 0.6);
    vec3 deepColor = vec3(0.0, 0.2, 0.3);
    vec3 waterColor = mix(shallowColor, deepColor, clamp(waterDepth/20.0, 0.0, 1.0));
    tempcol = mix(tempcol, waterColor, 0.3);
    tempcol += specularHighlights;

    float fin_alpha = clamp(waterDepth/5.0f, 0.0, 1.0);
    // tempcol = mix(tempcol, skycolor, alpha);
    // tempcol = (1-alpha)*tempcol + alpha*skycolor;
    fragcol = vec4(tempcol, fin_alpha);
}