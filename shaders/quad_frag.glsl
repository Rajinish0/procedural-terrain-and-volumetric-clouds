#version 430 core

in vec2 tCoord;

out vec4 col;

uniform sampler2D texture_diffuse1;

void main(){
    vec4 sp_col = texture(texture_diffuse1, tCoord);
    if (sp_col.a < 0.1) discard;
    col = sp_col; 
    // col = vec4( .23921f, .415686f, .44705f ,1.0f);
    // col = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}
