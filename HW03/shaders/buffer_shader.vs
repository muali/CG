#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

uniform mat4 transform;

out vec2 tex_coords;

void main()
{
	tex_coords = texCoords;
    gl_Position = transform * vec4(position, 1.f);
}