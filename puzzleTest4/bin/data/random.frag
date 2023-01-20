#version 300 es
// fragment shader

precision lowp float;

out vec4 out_Color;

uniform sampler2D texture_image;
uniform sampler2D texture_data;
uniform sampler2D texture_mask;
uniform vec2 resolution;
uniform vec2 puzzlePieces;

void main(){
	vec2 puzzlePieceSize = resolution / puzzlePieces;
	vec2 counter = floor(gl_FragCoord.xy / puzzlePieceSize) / resolution;
	
	vec2 offset1 = floor(texture(texture_data, counter).rg * puzzlePieces) * puzzlePieceSize;
	vec2 offset2 = floor(texture(texture_data, counter - vec2(1. / resolution.x, 0.)).rg * puzzlePieces) * puzzlePieceSize;
	vec2 offset3 = floor(texture(texture_data, counter - vec2(0., 1. / resolution.y)).rg * puzzlePieces) * puzzlePieceSize;
	
	vec4 col1 = texture(texture_image, fract((gl_FragCoord.xy + offset1) / resolution));
	vec4 col2 = texture(texture_image, fract((gl_FragCoord.xy + offset2) / resolution));
	vec4 col3 = texture(texture_image, fract((gl_FragCoord.xy + offset3) / resolution));
	
	vec4 mask = texture(texture_mask, vec2(mod(gl_FragCoord.xy, puzzlePieceSize) / (puzzlePieceSize + puzzlePieceSize / 4.)));
	
	if(mask.r > 0.){
		out_Color = vec4(col1);
	}else if(mask.g > 0.){
		out_Color = vec4(col2);
	}else if(mask.b > 0.){
		out_Color = vec4(col3);
	}
}
