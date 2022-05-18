#version 150
// fragment shader

out vec4 out_Color;

uniform sampler3D Tex0;
uniform vec3 resolution;
uniform int zCount;

vec4 color = texture(Tex0, vec3(gl_FragCoord.xy,gl_FragCoord.z + zCount) / resolution);
void main() {
	out_Color = color;
}
