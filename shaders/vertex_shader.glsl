#version 330 core

layout(location = 0) in vec2 _vPos; // Vertex coordinates
layout(location = 1) in vec2 _vUV; // Vertex's UV coordinates

uniform vec2 spriteSize; // Sprite's size
uniform vec2 position; // Coordinate of the entity in 2D world

out vec2 uv;

void main()
{
    // This is a simplified version of normal model matrix transform (GameJam style!)
    gl_Position = vec4(_vPos.xy * spriteSize, 0.0, 1.0); // Scale sprite
    gl_Position += vec4(position.xy, 0.0, 0.0); // Translate sprite
    uv = _vUV;
}


