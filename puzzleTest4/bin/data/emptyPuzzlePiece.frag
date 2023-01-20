#version 300 es
// fragment shader

precision lowp float;

out vec4 out_Color;

uniform sampler2D Tex0;
uniform vec2 resolution;
uniform vec2 puzzlePieces;
uniform vec2 position;

void main() {
	vec2 puzzlePieceSize = resolution / puzzlePieces;
	vec4 col0 = texture(Tex0, vec2(mod(gl_FragCoord.xy + puzzlePieceSize / 8. - position, resolution) / (puzzlePieceSize + puzzlePieceSize / 4.)));
	if(col0.a > 0.){
		out_Color = col0;
	}
}
