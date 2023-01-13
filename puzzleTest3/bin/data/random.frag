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
	vec4 col1;
	for(int i = 0; i < int(puzzlePieces.x * puzzlePieces.y); i++) {
		vec2 texcoord = vec2(float(i) + 0.5, 0.5) / vec2(puzzlePieces.x * puzzlePieces.y, 1.);
		vec4 pixelData = floor(texture(Tex1, texcoord) * 256.);
		float pixelValue = pixelData.r * 256. * 256. * 256. + pixelData.g * 256. * 256. + pixelData.b * 256. + pixelData.a;
		vec2 offset = vec2(mod(pixelValue, puzzlePieces.x) * puzzlePieceSize.x, floor(pixelValue / puzzlePieces.x) * puzzlePieceSize.y);
		vec2 position = vec2(mod(float(i), puzzlePieces.x) * puzzlePieceSize.x, floor(float(i) / puzzlePieces.x) * puzzlePieceSize.y);
		vec4 col0 = texture(Tex0, vec2(mod((gl_FragCoord.x + offset.x - position.x) / resolution.x, 1.), mod((gl_FragCoord.y + offset.y - position.y) / resolution.y, 1.)));
		vec4 col2 = texture(Tex2, vec2(mod(gl_FragCoord.x + puzzlePieceSize.x / 8. - position.x, resolution.x) / (puzzlePieceSize.x + puzzlePieceSize.x / 4.), mod(gl_FragCoord.y + puzzlePieceSize.y / 8. - position.y, resolution.y) / (puzzlePieceSize.y + puzzlePieceSize.y / 4.)));
		if(col2.a > 0.){
			col1 = col0;	
		}
	}
	out_Color = col1;
}
