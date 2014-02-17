#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;

uniform mat4 mvpMat;
out vec4 theColor;

void main(){
    gl_Position = mvpMat * position;
    //gl_Position = position
    theColor = color;
}
