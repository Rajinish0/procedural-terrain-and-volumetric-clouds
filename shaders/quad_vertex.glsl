#version 430 core
layout(location = 0) in vec2 inVec;
layout(location = 1) in vec2 texCoord;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;
uniform mat4 local = mat4(1.0f);

out vec2 tCoord;

void main() {
	gl_Position = proj*model*local*vec4(inVec.xy, 0.0, 1.0);
	tCoord = texCoord;
}