#version 330 core

uniform vec3 inColor; // Color vect from C++ program
out vec3 color;

void main()
{
    // Output color = red
    color = inColor;
}
