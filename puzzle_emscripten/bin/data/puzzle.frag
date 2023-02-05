#version 300 es
// fragment shader

precision lowp float;

out vec4 out_Color;

uniform sampler2D texture_image;
uniform sampler2D texture_data;
uniform vec2 resolution;
uniform vec2 puzzlePieces;
uniform vec2 puzzlePieceSize;

vec2 position;
vec2 offset;

void main() {
	position = floor(gl_FragCoord.xy / puzzlePieceSize) / puzzlePieces;
	offset = texture(texture_data, gl_FragCoord.xy / resolution).rg;

	if (mod(gl_FragCoord.x + 2., puzzlePieceSize.x) < 4. || mod(gl_FragCoord.y + 2., puzzlePieceSize.y) < 4.) {
		out_Color = vec4(0.);
    } else {
        out_Color = texture(texture_image, gl_FragCoord.xy / resolution + offset - position);
    }
}
