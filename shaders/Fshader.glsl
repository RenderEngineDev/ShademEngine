#version 460 core

out vec4 FragColor;
in vec3 FragPos;

void main()
{
    //FragColor = vec4(1.0); // set alle 4 vector values to 1.0
    FragColor = vec4(FragPos,1.0); // set alle 4 vector values to 1.0
}