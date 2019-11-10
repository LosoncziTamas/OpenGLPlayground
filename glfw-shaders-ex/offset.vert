#version 330 core
layout(location = 0) in vec3 position;

uniform float horizontalOffset;

void main()
{
    gl_Position = vec4(position.x + horizontalOffset, position.y, position.z, 1.0f);
}