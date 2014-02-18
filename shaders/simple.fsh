#version 330 core

in vec3 theColor; // Color vect from vertex shader...Ignore it, use blinking
uniform vec3 blinkingColor;
out vec3 outColor;

void main()
{
    outColor = blinkingColor;
}
