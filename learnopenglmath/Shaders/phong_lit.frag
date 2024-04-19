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
    vec4 position;
  
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

uniform DirectionLight directionLight;
uniform PointLight pointLight;
uniform Spotlight spotlight;
uniform Material material;
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 lightPos;

in vec3 normal;
in vec3 viewFragPos;
in vec3 viewSpotlightDir;
in vec3 viewLightPos;
in vec2 texUV;
in vec3 rawNormal;

vec3 getFaceColorFromNormal()
{
	return clamp(rawNormal + abs(rawNormal)*vec3(1.5), 0f, 1f);
}

vec3 getPhongDirectionLightColor()
{
	vec3 norm = normalize(normal);
	vec3 lightDirection = normalize(-viewLightPos);
	float diff = max(dot(norm, lightDirection), 0);
	vec3 reflected = reflect(-lightDirection, norm);
	float spec = pow(max(dot(normalize(-viewFragPos), reflected), 0), material.shininess);

	vec3 diffuse = directionLight.diffuse * diff * vec3(texture(material.diffuse, texUV));
	vec3 ambient = directionLight.ambient * vec3(texture(material.diffuse, texUV));
	vec3 specular = directionLight.specular * spec * vec3(texture(material.specular, texUV));

	vec3 resultColor = ambient + diffuse + specular;
	return resultColor;
}

vec3 getPhongPointLightColor()
{
	vec3 norm = normalize(normal);
	vec3 lightDirection = viewLightPos - viewFragPos;

	vec3 normLightDirection = normalize(lightDirection);
	float diff = max(dot(norm, normLightDirection), 0);
	vec3 reflected = reflect(-normLightDirection, norm);
	float spec = pow(max(dot(normalize(-viewFragPos), reflected), 0), material.shininess);

	float distanceToLight = length(lightDirection);
	float attenuation = 1.0/(pointLight.constant + pointLight.linear*distanceToLight + pointLight.quadratic*pow(distanceToLight, 2));
	
	vec3 ambient  = attenuation * pointLight.ambient  * vec3(texture(material.diffuse, texUV));
	vec3 diffuse  = attenuation * pointLight.diffuse  * diff * vec3(texture(material.diffuse, texUV));
	vec3 specular = attenuation * pointLight.specular * spec * vec3(texture(material.specular, texUV));

	vec3 resultColor = ambient + diffuse + specular;
	return resultColor;
}

vec3 unitVecToZeroToOne(vec3 unitVec)
{
	return (unitVec + vec3(1))/2;
}

vec3 getPhongSpotlightColor()
{
	vec3 norm = normalize(normal);
	vec3 lightDirection = viewLightPos - viewFragPos;

	vec3 normLightDirection = normalize(lightDirection);

	float angleToSpot = acos(dot(normalize(viewSpotlightDir), -normLightDirection));
	float spotCoef = (angleToSpot - spotlight.outerAngle) / (spotlight.innerAngle - spotlight.outerAngle);

	float diff = max(dot(norm, normLightDirection), 0);
	vec3 reflected = reflect(-normLightDirection, norm);
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
	fragColor = vec4(getPhongSpotlightColor(), 1);
}