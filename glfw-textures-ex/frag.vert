#version 330 core
out vec3 fragColor;

in vec2 texCoord;

uniform sampler2D texture;

void main()
{
    fragColor = texture(texture, texCoord);
}