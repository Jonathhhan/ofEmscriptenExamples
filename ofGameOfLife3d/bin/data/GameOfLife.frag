// fragment shader
#version 120

uniform sampler3D Tex0;
uniform vec3 resolution;
uniform int zCount;
uniform vec2 cells[27];

int get(int x, int y, int z) {
	return int(texture3D(Tex0, (gl_FragCoord.xyz + vec3(x, y, z - zCount)) / resolution).a);
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
		gl_FragColor = vec4(0.0, 0.0, 0.0, r.x);
	}
		else  {
			gl_FragColor = vec4(0.0, 0.0, 0.0, r.y);
		}
}
