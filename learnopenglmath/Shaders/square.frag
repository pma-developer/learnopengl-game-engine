#version 460


uniform sampler2D imageTex;

layout (location = 0) out vec4 resultColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

in vec3 normal;
in vec2 texCoord;
in vec3 fragPos;

void main()
{
    float ambientStrength = 0.05;
    vec3 ambient = ambientStrength * lightColor;

	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(lightPos - fragPos);
	float angleFactor = max(dot(norm, lightDir), 0.0);
	
	float specularStrength = 0.5;
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);
	vec3 specular = specularStrength * spec * lightColor;  

	vec3 diffuse = angleFactor * lightColor;
	
	vec3 texturedObjectColor = objectColor; //* vec3(texture(imageTex, texCoord));
    vec3 result = (ambient + diffuse * 0.6 + specular*1.1) * texturedObjectColor;//mix(ambientDiffuse, lightColor, specular);
	resultColor = vec4(result, 1);
}