#version 460

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexUV;
layout (location = 2) in vec3 aNormal;

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
uniform mat4 projection;
uniform mat4 view; 
uniform mat4 model;

out vec3 viewFragPos;
out vec3 normal;
out vec3 rawNormal;
out vec3 viewLightPos;
out vec3 viewSpotlightDir;
out vec2 texUV;

void main()
{
    normal = mat3(transpose(inverse(view * model))) * aNormal;
	rawNormal = aNormal;
	texUV = aTexUV;
	viewLightPos = vec3(view * vec4(spotlight.position, 1));
	viewSpotlightDir = vec3(view * vec4(spotlight.direction, 0));
	viewFragPos = vec3(view * model * vec4(aPos, 1));
	gl_Position = projection * view * model * vec4(aPos, 1);
	
}