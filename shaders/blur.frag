#version 130

uniform sampler2D texture;
uniform vec2 resolution;

void main()
{
	vec2 uv = gl_FragCoord.xy / resolution;
	vec4 color = vec4(0.0);

	float kernel[5] = float[](0.05, 0.1, 0.4, 0.1, 0.05);

	// Horizontal blur
	for (int i = -2; i <= 2; i++)
	{
		color += texture2D(texture, uv + vec2(i * 10, 0) / resolution) * kernel[i + 2];
	}

	// Vertical blur
	vec4 verticalBlur = vec4(0.0);
	for (int i = -2; i <= 2; i++)
	{
		verticalBlur += texture2D(texture, uv + vec2(0, i * 10) / resolution) * kernel[i + 2];
	}

	gl_FragColor = (color + verticalBlur) / 2.0;
}