#version 330 core

layout (location = 0) in vec3 cube_pos;   
layout (location = 1) in vec3 cube_color;

uniform mat4 objToWorld;
uniform mat4 worldToRaster;
uniform mat4 rotationMat;

out vec3 color;

void main()
{
	color = cube_color;
    gl_Position = worldToRaster * objToWorld * vec4(cube_pos, 1.0);
}
