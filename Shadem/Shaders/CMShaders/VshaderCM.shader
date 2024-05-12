#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec4 aValuesL;
layout (location = 4) in vec4 aValuesR;

out vec3 FragPos;
out vec2 TexCoords;
out vec3 Normal;
out vec4 valuesL;
out vec4 valuesR;

uniform mat4 model;
uniform mat4 MVP;

out fData
{
    vec3 position;
    vec3 normal;
    vec4 color;
}frag;

void main()
{
    valuesL = aValuesL;
    valuesR = aValuesR;
    
    frag.position = vec3(model * vec4(aPos, 1.0));
    frag.normal = aNormal;
    frag.color = model * vec4(aPos, 1.0f);

    FragPos = aPos;
	Normal = aNormal;
	TexCoords = aTexCoords;
	gl_Position = MVP * vec4(aPos, 1.0);
}