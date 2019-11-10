#version 330 core

layout(location = 0) in vec3 position;

out vec4 posColor;

void main()
{
    gl_Position = vec4(position, 1.0f);
    posColor = gl_Position;
}