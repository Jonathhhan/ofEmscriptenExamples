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
	vec4 col0 = texture(Tex0, vec2(mod((gl_FragCoord.xy + offsetA - positionA) / resolution, 1.)));
	vec4 col1 = texture(Tex1, vec2(mod(gl_FragCoord.xy + puzzlePieceSize / 8. - positionA, resolution) / (puzzlePieceSize + puzzlePieceSize / 4.)));
	vec4 col2 = texture(Tex0, vec2(mod((gl_FragCoord.xy + offsetB - positionB) / resolution, 1.)));
	vec4 col3 = texture(Tex1, vec2(mod(gl_FragCoord.xy + puzzlePieceSize / 8. - positionB, resolution) / (puzzlePieceSize + puzzlePieceSize / 4.)));
	if(col1.a > 0.){
		out_Color = col0;
	}
	if(col3.a > 0.){
		out_Color = col2;
	}
}
