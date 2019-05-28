#version 330 core

layout(location = 0) in vec2 _vPos; // Vertex coordinates
layout(location = 1) in vec2 _vUV; // Vertex's UV coordinates

out vec2 texture_UV;

uniform vec2 cameraRect; // Rectangle that the camera can see (in pixels)
uniform vec2 position; // Coordinate of the entity in 2D world
uniform vec2 scale;


void main()
{
    // DevNote:
    // This is a simplified version of model matrix transform.
    // You may see 'position' as the model matrix, using only the "translation" part

    // Model space to world space
    float new_x = _vPos.x * scale.x + position.x;
    float new_y = _vPos.y * scale.y + position.y;

    // Affine transform from screen space to NDC space
    new_x /= cameraRect.x;
    new_y /= cameraRect.y;

    // World space 0:0 is the screen center. NDC is from -1:1, meaning that, to
    // have 1 in NDC, the world coordinate is (position / (cameraRect/2)).
    // This is done by multiplying by 2 the previously divided value.
    new_x *= 2;
    new_y *= 2;

    gl_Position = vec4(new_x, new_y, 0.0, 1.0);
    texture_UV = _vUV;
}

