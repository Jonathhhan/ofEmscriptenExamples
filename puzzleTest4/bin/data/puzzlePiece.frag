#version 300 es
// fragment shader

precision lowp float;

out vec4 out_Color;

uniform sampler2D Tex0;
uniform sampler2D Tex1;
uniform vec2 resolution;
uniform vec2 puzzlePieces;
uniform vec2 offset;

void main() {
	vec2 puzzlePieceSize = resolution / puzzlePieces;
	vec4 col0 = texture(Tex0, vec2(mod((gl_FragCoord.xy - puzzlePieceSize / 16. + offset - (puzzlePieceSize / 16.)) / resolution, 1.)));
	vec4 col1 = texture(Tex1, vec2(gl_FragCoord.xy / (puzzlePieceSize +  puzzlePieceSize / 4.)));
	if(col1.r > 0. && col1.a > 0.){
		out_Color = col0;
	}
}
