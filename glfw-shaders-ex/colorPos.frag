#version 330 core

in vec4 posColor;

out vec4 fragmentColor;

void main()
{
    fragmentColor = posColor;
}