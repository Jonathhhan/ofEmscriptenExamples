#version 300 es
// fragment shader

precision lowp float;

out vec4 out_Color;

uniform sampler2D Tex0;
uniform sampler2D Tex1;
uniform vec2 resolution;
uniform vec2 puzzlePieceSize;
uniform vec2 puzzlePieces;
vec4 col1;

void main() {
	for(int i = 0; i < int(puzzlePieces.x * puzzlePieces.y); i++) {
		vec2 texcoord = vec2(float(i) + 0.5, 0.5) / vec2(puzzlePieces.x * puzzlePieces.y, 1.);
		float pixelValue = texture(Tex1, texcoord).r * 255. * 255. * 255. + texture(Tex1, texcoord).g * 255. * 255. + texture(Tex1, texcoord).b * 255.;
		vec2 offset = vec2(mod(pixelValue, puzzlePieces.x) * puzzlePieceSize.x, floor(pixelValue / puzzlePieces.x) * puzzlePieceSize.y);
		vec2 position = vec2(mod(float(i), puzzlePieces.x) * puzzlePieceSize.x, floor(float(i) / puzzlePieces.x) * puzzlePieceSize.y);
		vec4 col0 = texture(Tex0, vec2(mod((gl_FragCoord.x + offset.x - position.x) / resolution.x, 1.0), mod((gl_FragCoord.y + offset.y - position.y) / resolution.y, 1.0)));
		bool a = gl_FragCoord.x >= position.x && gl_FragCoord.x <= position.x + puzzlePieceSize.x && gl_FragCoord.y >= position.y && gl_FragCoord.y <= position.y + puzzlePieceSize.y;
		if(a){
			col1 = col0;
		}
	}
	out_Color = col1;
}
