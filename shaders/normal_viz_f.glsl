#version 430 core

in vec3 normal;
out vec4 fragcol;

void main() {
	fragcol = vec4( vec3(dot(vec3(0.0, 1.0, 0.0), normal))  , 1.0);
	// fragcol = vec4(1.0, 0.0, 0.0, 1.0);
}
