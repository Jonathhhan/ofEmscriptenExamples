#version 300 es
// fragment shader

precision lowp float;

out vec4 out_Color;

uniform sampler2D texture_image;
uniform sampler2D texture_data;
uniform sampler2D texture_duplicates;
uniform vec2 resolution;
uniform vec2 puzzlePieces;
uniform vec2 puzzlePieceSize;

vec2 position;
vec2 offset;
vec3 duplicates;
vec4 color;

void main() {
	position = floor(gl_FragCoord.xy / puzzlePieceSize) / puzzlePieces;
	offset = texture(texture_data, gl_FragCoord.xy / resolution).rg;
	duplicates = texture(texture_duplicates, gl_FragCoord.xy / resolution).rgb;

	if (mod(gl_FragCoord.x + 2., puzzlePieceSize.x) < 4. || mod(gl_FragCoord.y + 2., puzzlePieceSize.y) < 4.) {
		color = vec4(0.);
	} else if (duplicates.r == 1. && duplicates.g == 1.) {
		color = texture(texture_image, gl_FragCoord.xy / resolution + offset - position);
	} else if (duplicates.b == 1.) {
		color = vec4(1. - texture(texture_image, gl_FragCoord.xy / resolution + offset - position).rgb, 1.);
	} else {
		color = vec4(0.5);
	}
	
	out_Color = color;
}
