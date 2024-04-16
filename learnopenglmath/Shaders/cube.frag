#version 460

layout (location = 0) out vec4 fragColor;

uniform vec3 objectColor;
uniform vec3 lightPos;

in vec3 normal;
in vec3 fragPosition;
in vec2 texUV;

void main()
{
	vec3 lightDirection = normalize(fragPosition - lightPos);
	float diffuse = max(-dot(normal, lightDirection), 0);
	vec3 resultColor = diffuse * objectColor;
	fragColor = vec4(resultColor, 1);
}