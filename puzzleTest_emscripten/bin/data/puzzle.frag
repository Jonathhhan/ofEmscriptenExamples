#version 300 es
// fragment shader

out vec4 out_Color;

uniform sampler2D texture_image;
uniform sampler2D texture_data;
uniform sampler2D texture_mask;
uniform vec2 resolution;
uniform vec2 puzzlePieces;
uniform vec2 puzzlePieceSize;

vec2 counter;
vec2 position;
vec2 offset;
vec4 mask;
vec4 color;

vec4 get(float x, float y) {
	offset = vec2(texture(texture_data, (gl_FragCoord.xy + vec2(x, y) * puzzlePieceSize) / resolution).rg);
	position = vec2(counter + vec2(x, y) / puzzlePieces);
	return texture(texture_image, gl_FragCoord.xy / resolution + offset - position);
}

void main() {
	counter = floor(gl_FragCoord.xy / puzzlePieceSize) / puzzlePieces;
	mask = texture(texture_mask, (gl_FragCoord.xy / resolution - counter) * puzzlePieces);
	
	if(mask.r == 1. / 255.) {
		color = get(0., 0.);
	} else if (mask.r == 2. / 255.) {
		color = get(-1., 0.);
	} else if (mask.r == 3. / 255.) {
		color = get(0., -1.);
	} else if (mask.r == 4. / 255.) {
		color = get(-1., 0.);
	} else if (mask.r == 5. / 255.) {
		color = get(0., -1.);
	} else if (mask.r == 6. / 255.) {
		color = get(0., -1.);
	}

	out_Color = color;
}
