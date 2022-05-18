#version 150
// fragment shader

out vec4 out_Color;

uniform sampler3D Tex0;
uniform sampler2DRect Tex1;
uniform vec3 resolution;
uniform vec2 res;
uniform int zCount;
uniform vec2 cells[27];

int get(int x, int y, int z) {
	return int(texture(Tex0, (gl_FragCoord.xyz + vec3(x, y, z - zCount)) / resolution).a);
}
void main() {
vec4 col = texture(Tex1, gl_FragCoord.xy*(res.x/resolution.x));

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
		out_Color = vec4(col.r, col.g, col.b, r.x);
	}
		else  {
			out_Color = vec4(col.r, col.g, col.b, r.y);
		}
}

