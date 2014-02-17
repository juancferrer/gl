#version 330 core

in vec4 theColor; // Color vect from vertex shader...Ignore it, use blinking
uniform vec4 blinkingColor;
out vec4 outColor;

void main()
{
    outColor = blinkingColor;
}
