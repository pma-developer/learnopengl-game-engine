#version 460

in vec2 texCoord;

uniform sampler2D imageTex;

layout (location = 0) out vec4 diffuseColor;

void main()
{
	diffuseColor = texture(imageTex, texCoord);
}