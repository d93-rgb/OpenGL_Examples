#version 330 core

const vec2 quadVertices[3] = vec2[3]( vec2(0.0, 0.5), vec2(-0.5, -0.5), vec2(0.5, -0.5));

out vec3 color;

void main()
{
	color = vec3(0, 0, 1);
    gl_Position = vec4(quadVertices[gl_VertexID], 0.0, 1.0);
}
