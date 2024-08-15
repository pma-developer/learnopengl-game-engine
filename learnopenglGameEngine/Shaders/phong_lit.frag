#version 460

layout (location = 0) out vec4 fragColor;

struct Material
{
    sampler2D diffuse;
	sampler2D specular;
    float shininess;
}; 

struct DirectionLight
{
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight
{
    vec3 position;  
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
    float constant;
    float linear;
    float quadratic;
};

struct Spotlight
{
    vec3  position;

    vec3  direction;
    float innerAngle;
    float outerAngle;
	
    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
};

uniform Material material;
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 lightPos;

uniform DirectionLight directionLight;

#define NR_POINT_LIGHTS 4  
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform Spotlight spotlight;
in vec3 normal;
in vec3 viewFragPos;
in vec2 texUV;
in vec3 rawNormal;

vec3 getFaceColorFromNormal()
{
	return clamp(rawNormal + abs(rawNormal)*vec3(1.5), 0f, 1f);
}

vec3 getPhongDirectionLightColor(DirectionLight light, vec3 normal, vec3 fragPos)
{
	vec3 lightDirection = normalize(-light.direction);
	float diff = max(dot(normal, lightDirection), 0);
	vec3 reflected = reflect(-lightDirection, normal);
	float spec = pow(max(dot(normalize(-fragPos), reflected), 0), material.shininess);

	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texUV));
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, texUV));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, texUV));

	vec3 resultColor = ambient + diffuse + specular;
	return resultColor;
}

vec3 getPhongPointLightColor(PointLight pointLight, vec3 normal, vec3 fragPos)
{
	vec3 lightDirection = pointLight.position - fragPos;

	vec3 normLightDirection = normalize(lightDirection);
	float diff = max(dot(normal, normLightDirection), 0);
	vec3 reflected = reflect(-normLightDirection, normal);
	float spec = pow(max(dot(normalize(-fragPos), reflected), 0), material.shininess);

	float distanceToLight = length(lightDirection);
	float attenuation = 1.0/(pointLight.constant + pointLight.linear*distanceToLight + pointLight.quadratic*pow(distanceToLight, 2));
	
	vec3 ambient  = attenuation * pointLight.ambient  * vec3(texture(material.diffuse, texUV));
	vec3 diffuse  = attenuation * pointLight.diffuse  * diff * vec3(texture(material.diffuse, texUV));
	vec3 specular = attenuation * pointLight.specular * spec * vec3(texture(material.specular, texUV));

	vec3 resultColor = ambient + diffuse + specular;
	return resultColor;
}

vec3 getPhongSpotlightColor(Spotlight spotlight, vec3 normal, vec3 fragPos)
{
	vec3 lightDirection = spotlight.position - viewFragPos;

	vec3 normLightDirection = normalize(lightDirection);

	float angleToSpot = acos(dot(normalize(spotlight.direction), -normLightDirection));
	float spotCoef = clamp((angleToSpot - spotlight.outerAngle) /
	(spotlight.innerAngle - spotlight.outerAngle), 0., 1.);

	float diff = max(dot(normal, normLightDirection), 0);
	vec3 reflected = reflect(-normLightDirection, normal);
	float spec = pow(max(dot(normalize(-viewFragPos), reflected), 0), material.shininess);

	float distanceToLight = length(lightDirection);
	float attenuation = 1.0/(spotlight.constant + spotlight.linear*distanceToLight + spotlight.quadratic*pow(distanceToLight, 2));
	
	vec3 ambient  = spotlight.ambient * vec3(texture(material.diffuse, texUV));
	vec3 diffuse  = spotCoef * attenuation * spotlight.diffuse  * diff * vec3(texture(material.diffuse, texUV));
	vec3 specular = spotCoef * attenuation * spotlight.specular * spec * vec3(texture(material.specular, texUV));

	vec3 resultColor = ambient + diffuse + specular;
	return resultColor;
}

void main()
{
	vec3 result = vec3(0);
	vec3 norm = normalize(normal);
	result += getPhongDirectionLightColor(directionLight, norm, viewFragPos);
	result += getPhongSpotlightColor(spotlight, norm, viewFragPos);
	for(int i = 0; i < NR_POINT_LIGHTS; i++)
	{
		result += getPhongPointLightColor(pointLights[i], norm, viewFragPos);
	}

	fragColor = vec4(result, 1);
}