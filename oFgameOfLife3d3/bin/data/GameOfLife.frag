#version 150
// fragment shader

out vec4 out_Color;

uniform sampler3D Tex0;
uniform sampler3D Tex1;

uniform vec3 resolution;
uniform int zCount;
uniform vec2 cells[27];

int get(int x, int y, int z) {
	return int(texture(Tex0, (gl_FragCoord.xyz + vec3(x, y, z + zCount)) / resolution).a);
}

vec4 color = texture(Tex1, vec3(gl_FragCoord.xy, gl_FragCoord.z + zCount) / resolution);

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
		out_Color = vec4(color.rgb, r.x);
	}
		else  {
			out_Color = vec4(color.rgb , r.y);
		}

}

