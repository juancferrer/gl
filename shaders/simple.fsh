#version 330 core

in vec3 theColor; // Color vect from C++ program
out vec3 outColor;

void main()
{
    outColor = theColor;
}
