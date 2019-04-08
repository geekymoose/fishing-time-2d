#version 330 core

layout(location = 0) in vec2 _vPos; // Vertex coordinates
layout(location = 1) in vec2 _vUV; // Vertex's UV coordinates

out vec2 texture_UV;

uniform vec2 spriteSize; // Sprite's size
uniform vec2 position; // Coordinate of the entity in 2D world
uniform vec2 cameraRect; // Rect camera can see (in pixels)

void main()
{
    // This is a simplified version of normal model matrix transform (GameJam style!)

    // Position in world space
    float new_x = (_vPos.x * spriteSize.x) + position.x;
    float new_y = (_vPos.y * spriteSize.y) + position.y;

    // Affine transform from screen space to NDC space
    new_x /= cameraRect.x;
    new_y /= cameraRect.y;

    texture_UV = _vUV;
    gl_Position = vec4(new_x, new_y, 0.0, 1.0);
}


