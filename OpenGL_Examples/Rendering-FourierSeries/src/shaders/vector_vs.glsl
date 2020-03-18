#version 330 core

layout (location = 0) in vec2 pos;

uniform mat4 objToWorld;
uniform mat4 worldToRaster;

void main()
{
    gl_Position = worldToRaster * objToWorld * vec4(pos, 0, 1);
}
