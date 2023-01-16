#version 300 es
// fragment shader

precision lowp float;

out vec4 out_Color;

uniform sampler2D Tex0;
uniform sampler2D Tex1;
uniform sampler2D Tex2;
uniform vec2 resolution;
uniform vec2 puzzlePieces;

void main(){
	vec2 puzzlePieceSize = resolution / puzzlePieces;
	vec2 counter = floor((1. - gl_FragCoord.xy / resolution) * puzzlePieces);
	vec2 texcoord = vec2(counter.x + 0.5, 0.5) / vec2(puzzlePieces.x * puzzlePieces.y, 1.);
	vec4 pixelData = floor(texture(Tex1, texcoord) * 256.);
	float pixelValue = pixelData.r * 256. * 256. * 256. + pixelData.g * 256. * 256. + pixelData.b * 256. + pixelData.a;
	vec2 offset = vec2(mod(pixelValue, puzzlePieces.x) * puzzlePieceSize.x, floor(pixelValue / puzzlePieces.x) * puzzlePieceSize.y);
	vec2 position = vec2(mod(counter.x, puzzlePieces.x) * puzzlePieceSize.x, floor(counter.y / puzzlePieces.x) * puzzlePieceSize.y);
	vec4 col0 = texture(Tex0, vec2(mod((gl_FragCoord.xy + offset - position) / resolution, 1.)));
	vec4 col2 = texture(Tex2, vec2(mod(gl_FragCoord.xy + puzzlePieceSize / 8., puzzlePieceSize) / (puzzlePieceSize + puzzlePieceSize / 4.)));
	out_Color = vec4(col0.rgb, col2.a);
}
