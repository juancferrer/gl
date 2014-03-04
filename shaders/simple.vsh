#version 330 core

layout(location = 0) in vec3 vertexPosition_ms;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 vertexNormal_ms;

uniform mat4 mvpMat;

out vec2 UV;

void main(){
    vec4 pos = vec4(vertexPosition_ms, 1); // Turn vec3 position into vec4 position (add w)
    gl_Position = mvpMat * pos;
    UV = vertexUV;
}
