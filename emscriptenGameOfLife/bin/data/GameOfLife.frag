#version 300 es
// fragment shader

precision lowp float;

out vec4 out_Color;

uniform sampler2D Tex0;
uniform vec2 resolution;
uniform float lCell_0;
uniform float lCell_1;
uniform float lCell_2;
uniform float lCell_3;
uniform float lCell_4;
uniform float lCell_5;
uniform float lCell_6;
uniform float lCell_7;
uniform float lCell_8;
uniform float dCell_0;
uniform float dCell_1;
uniform float dCell_2;
uniform float dCell_3;
uniform float dCell_4;
uniform float dCell_5;
uniform float dCell_6;
uniform float dCell_7;
uniform float dCell_8;

int get(int x, int y) {
	return int(texture(Tex0, (gl_FragCoord.xy + vec2(x, y)) / resolution).a);
}

void main() {
vec2 rule[9] = vec2[9](
	vec2(lCell_0, dCell_0),
	vec2(lCell_1, dCell_1),
	vec2(lCell_2, dCell_2),
	vec2(lCell_3, dCell_3),
	vec2(lCell_4, dCell_4),
	vec2(lCell_5, dCell_5),
	vec2(lCell_6, dCell_6),
	vec2(lCell_7, dCell_7),
	vec2(lCell_8, dCell_8)
);
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
		out_Color = vec4(0., 0., 0., r.x);
	}
		else  {
			out_Color = vec4(0., 0., 0., r.y);
		}
}
