#version 330 core

out vec4 fragcol;

in vec4 finPos;

uniform sampler2D t1;
uniform sampler2D t2;



void main(){
    vec2 tCoord = ((finPos/finPos.w).xy + 1)/2.0f;
    fragcol = mix(texture(t1, vec2(tCoord.x, 1-tCoord.y)), texture(t2, tCoord), 0.5f);
    // fragcol = vec4(0.0f, 0.0f, 1.0f, 1.0f);
}