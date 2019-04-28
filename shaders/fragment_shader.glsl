#version 330 core

in vec2 texture_UV;
out vec4 fragColor;

uniform sampler2D tex;


void main()
{
    fragColor = texture(tex, texture_UV);
}

