#version 430 core

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT{
	vec3 normal;
} vs_out[];

const float MAG = 1.;

out vec3 normal;

uniform mat4 proj;

void genLine(int index) { // for the ith vertex
	gl_Position = proj * gl_in[index].gl_Position;
	EmitVertex();

	gl_Position = proj * ( gl_in[index].gl_Position + 
								 vec4(vs_out[index].normal, 0.0) * MAG);
	normal = vs_out[index].normal;
	EmitVertex();

	EndPrimitive();

}

void main() {
	for (int i = 0; i < 3; ++i)
		genLine(i);
}