#version 300 es
// fragment shader

precision lowp float;

out vec4 out_Color;

uniform sampler2D Tex0;
uniform vec2 resolution;
uniform float gridNumX;
uniform float gridNumY;

void main() {
    vec4 col0 = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 col1 = vec4(1.0, 1.0, 1.0, 0.0);
    bool a = int(mod(gl_FragCoord.x + 1., resolution.x / gridNumX)) == 0 || int(mod(gl_FragCoord.x + 1., resolution.x / gridNumX)) - 1 == 0;
    bool b = int(mod(gl_FragCoord.y + 1., resolution.y / gridNumY)) == 0 || int(mod(gl_FragCoord.y + 1., resolution.y / gridNumY)) - 1 == 0;
    out_Color = (a || (!a && b)) ? col0 : col1;
}
