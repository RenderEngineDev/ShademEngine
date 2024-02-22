#version 460 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

#pragma include "../Shadem/Shaders/BasicShader/FongLight.shader"

in fData
{
    vec3 position;
    vec3 normal;
    vec4 color;
}frag;

uniform vec3 cameraPosition;

void main()
{
    vec3 result = calculateFongLights(frag.normal, cameraPosition, frag.position, dirLight, pointLights, spotLight);
    FragColor = vec4(result, 1.0f);
    //FragColor = vec4(FragPos, 1.0f);
}