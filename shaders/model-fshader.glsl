#version 430 core

in vec2 texCoord;
out vec4 fcol;

uniform sampler2D texture_diffuse1;
uniform float opacity = 1.0;

uniform vec3 sunDirection;
in vec3 normal;

void main()
{
	vec4 tcol = texture(texture_diffuse1, texCoord);
	float finAlpha = tcol.a * opacity;
	if (finAlpha < 0.5) 
		discard;
	fcol = vec4(tcol.rgb, finAlpha) * max(dot(normal, sunDirection), 0.0f) + 
		   vec4(tcol.rgb * .5, finAlpha);
	// fcol = tcol;
}