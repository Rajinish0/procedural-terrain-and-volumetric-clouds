#version 330 core
out vec4 FragColor;
uniform float testFloat;
uniform int testInt;
uniform bool testBool;
uniform vec2 testVec2;
uniform vec3 testVec3;
uniform vec4 testVec4;
void main() {
    FragColor = vec4(1.0);
}