#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

out vec2 tex_coords;
out vec4 proj_coords;

uniform mat4 transform;
uniform mat4 proj_transform;

void main()
{
    gl_Position = transform * vec4(position, 1.f);
    tex_coords = texCoords;
	proj_coords = proj_transform * vec4(position, 1.f);
}