#version 330 core

layout(location = 0) in vec3 position;

uniform mat4 transform;
uniform mat4 inv_proj_transform;

void main()
{
    gl_Position = transform * inv_proj_transform * vec4(position, 1.0);
}

