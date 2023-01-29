#version 150
// fragment shader

out vec4 out_Color;

uniform sampler2D texture_image;
uniform sampler2D texture_data;
uniform sampler2D texture_mask;
uniform vec2 resolution;
uniform vec2 puzzlePieces;

vec2 puzzlePieceSize;
vec2 counter;
vec4 mask;
vec2 position_offset;
vec2 position;
vec2 offset;

void main() {
	puzzlePieceSize = resolution / puzzlePieces;
	counter = floor(gl_FragCoord.xy / puzzlePieceSize) / puzzlePieces;
	mask = texture(texture_mask, (gl_FragCoord.xy / resolution - counter) * puzzlePieces);
	
	if(mask.r < 1.5 / 255.) {
		position = counter;
		offset = texelFetch(texture_data, ivec2(gl_FragCoord.xy), 0).rg;
	} else if (mask.r < 2.5 / 255.) {
		position_offset = vec2(-1., 0.);
		position = vec2(counter.x + position_offset.x / puzzlePieces.x, counter.y);
		offset = texelFetch(texture_data, ivec2(gl_FragCoord.xy + position_offset * puzzlePieceSize), 0).rg;
	} else if (mask.r < 3.5 / 255.) {
		position_offset = vec2(0., -1.);
		position = vec2(counter.x, counter.y + position_offset.y / puzzlePieces.y);
		offset = texelFetch(texture_data, ivec2(gl_FragCoord.xy + position_offset * puzzlePieceSize), 0).rg;
	} else if (mask.r < 4.5 / 255.) {
		position_offset = vec2(1., 0.);
		position = vec2(counter.x + position_offset.x / puzzlePieces.x, counter.y);
		offset = texelFetch(texture_data, ivec2(gl_FragCoord.xy + position_offset * puzzlePieceSize), 0).rg;
	} else if (mask.r < 5.5 / 255.) {
		position_offset = vec2(0., 1.);
		position = vec2(counter.x, counter.y + position_offset.y / puzzlePieces.y);
		offset = texelFetch(texture_data, ivec2(gl_FragCoord.xy + position_offset * puzzlePieceSize), 0).rg;
	}

	out_Color = texture(texture_image, gl_FragCoord.xy / resolution + offset - position);
}
