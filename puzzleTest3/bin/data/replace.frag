#version 300 es
// fragment shader

precision lowp float;

out vec4 out_Color;

uniform sampler2D Tex0;
uniform vec2 resolution;
uniform vec2 puzzlePieceSize;
uniform vec2 offsetA;
uniform vec2 positionA;
uniform vec2 offsetB;
uniform vec2 positionB;

void main() {
	vec4 col0 = texture(Tex0, vec2(mod((gl_FragCoord.x + offsetA.x - positionA.x) / resolution.x, 1.0), mod((gl_FragCoord.y + offsetA.y - positionA.y) / resolution.y, 1.0)));
	bool a = gl_FragCoord.x > positionA.x && gl_FragCoord.x < positionA.x + puzzlePieceSize.x && gl_FragCoord.y > positionA.y && gl_FragCoord.y < positionA.y + puzzlePieceSize.y;
	vec4 col1 = texture(Tex0, vec2(mod((gl_FragCoord.x + offsetB.x - positionB.x) / resolution.x, 1.0), mod((gl_FragCoord.y + offsetB.y - positionB.y) / resolution.y, 1.0)));
	bool b = gl_FragCoord.x > positionB.x && gl_FragCoord.x < positionB.x + puzzlePieceSize.x && gl_FragCoord.y > positionB.y && gl_FragCoord.y < positionB.y + puzzlePieceSize.y;
	if(a){
		out_Color = col0;
	}
	if(b){
		out_Color = col1;
	}
}
