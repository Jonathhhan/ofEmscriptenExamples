#version 300 es
// vertex shader

uniform mat4 modelViewProjectionMatrix;
in vec4 position;
in vec2 texcoord;

out vec2 texCoordVarying;
out vec4 gl_Position;

void main()
{
	texCoordVarying = texcoord;
	gl_Position = modelViewProjectionMatrix * position;
}
