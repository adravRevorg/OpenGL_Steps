#version 330 core
out vec4 FragColor;
  
in vec3 color;
in vec2 TexCoord;

uniform float fraction;
uniform sampler2D texture1;

void main()
{
    FragColor = vec4(color, 1.0f);
}