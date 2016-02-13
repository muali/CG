#version 330

in vec2 tex_coords;

layout (location = 0) out vec3 color;

uniform sampler2D texture_diffuse1;

void main()
{
	color = texture(texture_diffuse1, tex_coords).rgb;
}