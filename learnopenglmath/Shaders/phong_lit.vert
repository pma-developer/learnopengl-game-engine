#version 460

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexUV;
layout (location = 2) in vec3 aNormal;

struct Light
{
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;
uniform mat4 projection;
uniform mat4 view; 
uniform mat4 model;

out vec3 viewFragPos;
out vec3 normal;
out vec3 rawNormal;
out vec3 viewLightPos;
out vec2 texUV;

void main()
{
    normal = mat3(transpose(inverse(view* model))) * aNormal;
	rawNormal = aNormal;
	texUV = aTexUV;
	viewLightPos = vec3(view * vec4(light.position, 1));
	viewFragPos = vec3(view * model * vec4(aPos, 1));
	gl_Position = projection * view * model * vec4(aPos, 1);
	
}