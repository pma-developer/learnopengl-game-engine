#version 460

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexUV;
layout (location = 2) in vec3 aNormal;

uniform mat4 projection;
uniform mat4 view; 
uniform mat4 model;

out vec3 fragPosition;
out vec3 normal;
out vec2 texUV;

//    lightSourceShader.setMat4("view", mainCamera->getViewMatrix());
//    lightSourceShader.setMat4("projection", mainCamera->getProjectionMatrix());
//    lightSourceShader.setVec3("lightColor", lightColor);
//    lightSourceShader.setMat4("model", model);

void main()
{
	normal = aNormal;
	texUV = aTexUV;
	fragPosition = vec3(model * vec4(aPos, 1));
	gl_Position = projection * view * model * vec4(aPos, 1);
	
}