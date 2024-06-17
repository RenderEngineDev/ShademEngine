#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;

out vec2 TexCoords;

void main()
{ 
    FragPos = aPos;
    float ang = radians(90.0f);
    mat3 rotation = mat3(1.0f, 0.0f, 0.0f,
                         0.0f, cos(ang), -sin(ang),
                         0.0f, sin(ang), cos(ang));
    
	gl_Position = vec4(aPos * rotation, 1.0);
    TexCoords = aTexCoords;
}