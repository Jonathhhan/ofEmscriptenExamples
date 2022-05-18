#version 150
// fragment shader

out vec4 out_Color;

uniform sampler3D Tex0;
uniform sampler2DRect Tex1;
uniform vec3 resolution;
uniform vec2 res;
uniform int zCount;


void main() {
vec4 color = texture(Tex0, vec3(gl_FragCoord.xy, gl_FragCoord.z+zCount)/ resolution);
vec4 col = texture(Tex1, gl_FragCoord.xy*(res.x/resolution.x));
	out_Color = vec4(col.rgb,color.a);
}
