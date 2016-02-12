#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

out vec2 TexCoords;
out vec4 ProjCoords;

uniform mat4 transform;
uniform mat4 proj_transform;

void main()
{
    gl_Position = transform * vec4(position, 1.f);
    TexCoords = texCoords;
	ProjCoords = proj_transform * vec4(position, 1.f);
}