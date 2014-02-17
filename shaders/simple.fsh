#version 330 core

in vec3 theColor; // Color vect from vertex shader
out vec3 outColor;

void main()
{
    outColor = theColor;
}
