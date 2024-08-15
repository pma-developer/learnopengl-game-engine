#version 460

layout (location = 0) out vec4 diffuseColor;

uniform vec3 lightColor;

void main()
{
	diffuseColor = vec4(lightColor, 1);
}