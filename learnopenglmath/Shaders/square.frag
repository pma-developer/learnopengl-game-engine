#version 460

in vec2 texCoord;

uniform sampler2D imageTex;

layout (location = 0) out vec4 diffuseColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

in vec3 normal;

void main()
{
    float ambientStrength = 0.1;
	
    vec3 ambient = ambientStrength * lightColor;
	
    vec3 result = ambient * objectColor;
	diffuseColor = vec4(result, 1) * texture(imageTex, texCoord);
}