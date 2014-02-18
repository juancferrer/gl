#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

uniform mat4 mvpMat;
out vec3 theColor;

void main(){
    vec4 pos = vec4(position, 1); // Turn vec3 position into vec4 position (add w)
    gl_Position = mvpMat * pos;
    theColor = color;
}
