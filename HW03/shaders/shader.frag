#version 330

in vec2 tex_coords;
in vec4 proj_coords;

out vec4 color;

uniform sampler2D texture_diffuse1;

uniform sampler2D projective_texture;

void main()
{
	vec2 ndc = (proj_coords.xy / proj_coords.w) / 2.0 + 0.5;
	vec4 proj_color = texture(projective_texture, vec2(ndc.x, 1.0 - ndc.y));
	color = texture(texture_diffuse1, tex_coords) + 0.4 * proj_color;
}