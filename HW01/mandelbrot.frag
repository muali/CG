uniform sampler1D colors;
uniform vec2 coord;
uniform float zoom;
uniform float range;
uniform int max_step;

void main(void)
{
	vec2 p = gl_FragCoord.xy;

	p.x = coord.x + p.x * zoom;
	p.y = coord.y + p.y * zoom;

	vec2 z = p;
	int i;
	for (i = 0; i < max_step; ++i)
	{
		float x = (z.x * z.x - z.y * z.y) + p.x;
		float y = 2. * z.x * z.y + p.y;
		if ((x * x + y * y) > range) break;
		z.x = x;
		z.y = y;
	}
	gl_FragColor = texture(colors, i / float(max_step));
}

