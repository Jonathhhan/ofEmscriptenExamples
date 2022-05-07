#version 150
// fragment shader

out vec4 out_Color;

uniform sampler2DRect Tex0;
uniform vec2 resolution;
uniform vec2 cell_0;
uniform vec2 cell_1;
uniform vec2 cell_2;
uniform vec2 cell_3;
uniform vec2 cell_4;
uniform vec2 cell_5;
uniform vec2 cell_6;
uniform vec2 cell_7;
uniform vec2 cell_8;
vec2 rule[9] = vec2[9](
	cell_0,
	cell_1,
	cell_2,
	cell_3,
	cell_4,
	cell_5,
	cell_6,
	cell_7,
	cell_8
);
	
int get(int x, int y) {
	return int(texture(Tex0, mod(gl_FragCoord.xy + vec2(x, y), resolution)).a);
}

void main() {
int sum = 
	get(-1, -1) +
	get(-1,  0) +
	get(-1,  1) +
	get( 0, -1) +
	get( 0,  1) +
	get( 1, -1) +
	get( 1,  0) +
	get( 1,  1);
	vec2 r = rule[sum];
	if (get(0, 0) == 1) {
		out_Color = vec4(0.0, 0.0, 0.0, r.x);
	}
		else  {
			out_Color = vec4(0.0, 0.0, 0.0, r.y);
		}
}

