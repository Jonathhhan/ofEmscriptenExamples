#version 300 es
// fragment shader

precision lowp float;

out vec4 out_Color;

uniform sampler2D Tex0;
uniform vec2 resolution;
uniform vec2 gridNums;

void main() {
	if (mod(gl_FragCoord.x + 1., resolution.x / gridNums.x) < 2. || mod(gl_FragCoord.y + 1., resolution.y / gridNums.y) < 2.) {
		out_Color = vec4(0.0, 0.0, 0.0, 1.0);
	} else {
		out_Color = vec4(1.0, 1.0, 1.0, 0.0);
	}
}
