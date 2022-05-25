#version 150
// fragment shader

out vec4 out_Color;

uniform sampler3D Tex0;
uniform sampler2DRect Tex1;

uniform vec3 resolution;
uniform int zCount;

vec4 color1 = texture(Tex0, vec3(gl_FragCoord.xy, gl_FragCoord.z + zCount) / resolution);

vec4 color2 = texture(Tex1, gl_FragCoord.xy);

void main() {
	out_Color = vec4(color2.rgb, color1.a);
}
