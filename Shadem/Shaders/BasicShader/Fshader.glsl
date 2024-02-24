#version 460 core

out vec4 FragColor;
in vec3 FragPos;

in vec2 TexCoord;

uniform int TextureQuantity;
uniform sampler2D Texture;

void main()
{
    //FragColor = vec4(1.0); // set alle 4 vector values to 1.0
    //FragColor = vec4(FragPos,1.0); // set alle 4 vector values to 1.0

    if( TextureQuantity > 0 )
    {
        FragColor = texture(Texture, TexCoord);
    }
    else
    {
        FragColor = vec4(FragPos,1.0); // set alle 4 vector values to 1.0
    }
}