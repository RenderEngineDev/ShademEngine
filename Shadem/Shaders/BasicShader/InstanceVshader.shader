#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in mat4 instanceTransform;

uniform mat4 MVP;

out vec3 FragPos;
out vec2 TexCoords;
out vec3 Normal;

void main()
{
	FragPos = aPos;
	Normal = aNormal;
	TexCoords = aTexCoords;
	gl_Position = MVP * instanceTransform * vec4(aPos, 1.0);
}