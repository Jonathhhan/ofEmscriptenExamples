#version 300 es
// fragment shader

precision lowp float;

out vec4 out_Color;

uniform sampler2D Tex0;
uniform sampler2D Tex1;
uniform vec2 resolution;
uniform vec2 puzzlePieces;
uniform vec2 offsetA;
uniform vec2 positionA;
uniform vec2 offsetB;
uniform vec2 positionB;

void main() {
	vec2 puzzlePieceSize = resolution / puzzlePieces;
	vec4 col0 = texture(Tex0, vec2(mod((gl_FragCoord.x + offsetA.x - positionA.x) / resolution.x, 1.), mod((gl_FragCoord.y + offsetA.y - positionA.y) / resolution.y, 1.)));
	vec4 col1 = texture(Tex1, vec2(mod(gl_FragCoord.x + puzzlePieceSize.x / 8. - positionA.x, resolution.x) / (puzzlePieceSize.x + puzzlePieceSize.x / 4.), mod(gl_FragCoord.y + puzzlePieceSize.y / 8. - positionA.y, resolution.y) / (puzzlePieceSize.y + puzzlePieceSize.y / 4.)));
	vec4 col2 = texture(Tex0, vec2(mod((gl_FragCoord.x + offsetB.x - positionB.x) / resolution.x, 1.), mod((gl_FragCoord.y + offsetB.y - positionB.y) / resolution.y, 1.)));
	vec4 col3 = texture(Tex1, vec2(mod(gl_FragCoord.x + puzzlePieceSize.x / 8. - positionB.x, resolution.x) / (puzzlePieceSize.x + puzzlePieceSize.x / 4.), mod(gl_FragCoord.y + puzzlePieceSize.y / 8. - positionB.y, resolution.y) / (puzzlePieceSize.y + puzzlePieceSize.y / 4.)));
	if(col1.a > 0.){
		out_Color = col0;
	}
	if(col3.a > 0.){
		out_Color = col2;
	}
}
