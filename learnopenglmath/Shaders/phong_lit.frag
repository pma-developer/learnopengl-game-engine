#version 460

layout (location = 0) out vec4 fragColor;

struct Material
{
    sampler2D diffuse;
	sampler2D specular;
    float shininess;
}; 

struct Light
{
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;
uniform Material material;
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 lightPos;

in vec3 normal;
in vec3 viewFragPos;
in vec3 viewLightPos;
in vec2 texUV;
in vec3 rawNormal;

vec3 getFaceColorFromNormal()
{
	return clamp(rawNormal + abs(rawNormal)*vec3(1.5), 0f, 1f);
}

vec3 getPhongLightingColor()
{
	vec3 norm = normalize(normal);
	vec3 lightDirection = normalize(viewLightPos - viewFragPos);
	float diff = max(dot(norm, lightDirection), 0);
	vec3 reflected = reflect(-lightDirection, norm);
	float spec = pow(max(dot(normalize(-viewFragPos), reflected), 0), material.shininess);

	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texUV));
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, texUV));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, texUV));

	vec3 resultColor = ambient + diffuse + specular;
	return resultColor;
}

void main()
{
	fragColor = vec4(getPhongLightingColor(), 1);
}