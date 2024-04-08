#version 460

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

uniform mat4 transform;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 texCoord;
out vec2 normal;

void main()
{
	texCoord = aTexCoord;
	gl_Position = projection * view * model * vec4(aPos, 1);
}