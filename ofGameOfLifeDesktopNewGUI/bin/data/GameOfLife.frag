#version 150
// fragment shader

out vec4 out_Color;

uniform sampler2DRect Tex0;
uniform vec2 resolution;
uniform vec2 cells[9];

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
	vec2 r = cells[sum];
	if (get(0, 0) == 1) {
		out_Color = vec4(0.0, 0.0, 0.0, r.x);
	}
		else  {
			out_Color = vec4(0.0, 0.0, 0.0, r.y);
		}
}
