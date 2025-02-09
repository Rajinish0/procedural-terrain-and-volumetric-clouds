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
uniform float moveFac;
uniform vec3 planeNorm;
uniform vec3 camPos;

const float distortStrength = 0.05f;
const float maxDist = 20.0f;


void main(){

    vec2 distort = (texture(dudv, vec2(tCoord.x + moveFac, tCoord.y)).rg*2 - 1) * distortStrength;
    vec2 distort2 = (texture(dudv, vec2(-tCoord.x + moveFac, tCoord.y + moveFac)).rg*2 - 1) * distortStrength;
    vec2 distortT = distort + distort2;

    vec3 diff = normalize(camPos - worldPos.xyz);
    float d = max(dot(planeNorm, diff), 0.0f);

    
    vec2 tC = ((finPos/finPos.w).xy + 1.0f)/2.0f;
    vec2 reflect_c = vec2(tC.x, 1-tC.y);
    vec2 refrect_c = tC;

    reflect_c += distortT;
    refrect_c += distortT;

    reflect_c = clamp(reflect_c, 0.001, 0.999);
    refrect_c = clamp(refrect_c, 0.001, 0.999);
    float di = min(distFromCamera, maxDist);
    float alpha = di / maxDist;


    // fragcol = mix(texture(t1, reflect_c), texture(t2, refrect_c), d);
    // fragcol = mix(fragcol, vec4(0.0, 0.3, 0.5, 1.0), 0.2);
    vec3 tempcol = mix(texture(t1, reflect_c), texture(t2, refrect_c), d).xyz;
    tempcol = mix(tempcol, vec3(0.0, 0.3, 0.5), 0.2);
    // tempcol = mix(tempcol, skycolor, alpha);
    // tempcol = (1-alpha)*tempcol + alpha*skycolor;
    fragcol = vec4(tempcol, 1.0f);
}