#version 150
// fragment shader

out vec4 out_Color;

uniform sampler2D texture_image;
uniform sampler2D texture_data;
uniform sampler2D texture_mask;
uniform vec2 resolution;
uniform vec2 puzzlePieces;
uniform vec2 puzzlePieceSize;

vec2 counter;
vec4 mask;
vec2 position_offset;
vec2 position;
vec2 offset;

void main() {
	counter = floor(gl_FragCoord.xy / puzzlePieceSize) / puzzlePieces;
	mask = texture(texture_mask, (gl_FragCoord.xy / resolution - counter) * puzzlePieces);
	
	if(mask.r == 1. / 255.) {
		position = counter;
		offset = texture(texture_data, gl_FragCoord.xy / resolution).rg;
	} else if (mask.r == 2. / 255.) {
		position_offset = vec2(-1., 0.);
		position = vec2(counter.x + position_offset.x / puzzlePieces.x, counter.y);
		offset = texture(texture_data, (gl_FragCoord.xy + position_offset * puzzlePieceSize) / resolution).rg;
	} else if (mask.r == 3. / 255.) {
		position_offset = vec2(0., -1.);
		position = vec2(counter.x, counter.y + position_offset.y / puzzlePieces.y);
		offset = texture(texture_data, (gl_FragCoord.xy + position_offset * puzzlePieceSize) / resolution).rg;
	} else if (mask.r == 4. / 255.) {
		position_offset = vec2(1., 0.);
		position = vec2(counter.x + position_offset.x / puzzlePieces.x, counter.y);
		offset = texture(texture_data, (gl_FragCoord.xy + position_offset * puzzlePieceSize) / resolution).rg;
	} else if (mask.r == 5. / 255.) {
		position_offset = vec2(0., 1.);
		position = vec2(counter.x, counter.y + position_offset.y / puzzlePieces.y);
		offset = texture(texture_data, (gl_FragCoord.xy + position_offset * puzzlePieceSize) / resolution).rg;
	}

	out_Color = texture(texture_image, gl_FragCoord.xy / resolution + offset - position);
}