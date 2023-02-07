#version 300 es

precision lowp float;
precision lowp sampler3D;

out vec4 out_Color;

uniform sampler3D Tex0;
uniform vec3 resolution;
uniform int zCount;
uniform vec2 cells[27];

int get(int x, int y, int z) {
	return int(texture(Tex0, (gl_FragCoord.xyz + vec3(x, y, z - zCount)) / resolution).a);
}

void main() {
	int sum = 
	get(-1, -1, -1) +
	get(-1,  0, -1) +
	get(-1,  1, -1) +
	get( 0, -1, -1) +
	get( 0,  1, -1) +
	get( 1, -1, -1) +
	get( 1,  0, -1) +
	get( 1,  1, -1) +
	get( 0,  0, -1) +
	get(-1, -1, 0) +
	get(-1,  0, 0) +
	get(-1,  1, 0) +
	get( 0, -1, 0) +
	get( 0,  1, 0) +
	get( 1, -1, 0) +
	get( 1,  0, 0) +
	get( 1,  1, 0) +
	get(-1, -1, 1) +
	get(-1,  0, 1) +
	get(-1,  1, 1) +
	get( 0, -1, 1) +
	get( 0,  1, 1) +
	get( 1, -1, 1) +
	get( 1,  0, 1) +
	get( 1,  1, 1) +
	get( 0,  0, 1);
	vec2 r = cells[sum];
	if (get(0, 0, 0) == 1) {
		out_Color = vec4(0.0, 0.0, 0.0, r.x);
	}
		else  {
			out_Color = vec4(0.0, 0.0, 0.0, r.y);
		}
}
