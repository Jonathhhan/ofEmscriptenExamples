#version 300 es

precision lowp float;
precision lowp sampler3D;

out vec4 out_Color;

uniform sampler3D Tex0;
uniform vec3 resolution;
uniform int zCount;

int get(int x, int y, int z) {
	return int(texture(Tex0, (gl_FragCoord.xyz + vec3(x, y, z - zCount)) / resolution).a);
}

void main() {
	out_Color = vec4(0.0, 0.0, 0.0, get(0,0,0));
}
