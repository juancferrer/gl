#version 330 core

in vec2 UV;

uniform sampler2D sampler;

out vec3 outColor;


void main()
{
    outColor = texture2D(sampler, UV).rgb;
}
