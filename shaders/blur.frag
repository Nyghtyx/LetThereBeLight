#version 130

uniform sampler2D lightMap;
uniform vec2 resolution;
uniform int baseBlurRadius;
uniform vec2 lightPos;

void main()
{
	vec2 uv = gl_FragCoord.xy / resolution;
	vec2 normPos = lightPos.xy / resolution;
	float distanceToLight = length(uv - normPos);
	float blurRadius = baseBlurRadius * distanceToLight;
	vec4 color = vec4(0.0);
	int samples = 0;

	if (texture2D(lightMap, uv).r == 0.0)
	{		
		// Horizontal blur
		for (int i = -2; i <= 2; i++)
		{
			vec4 pixel = texture2D(lightMap, uv + vec2(i * blurRadius, 0) / resolution);
			if (pixel.r == 0.0)
			{
				color += vec4(pixel.rgb / 2.0, pixel.a);
			}
			else
			{
				color += pixel;
			}
			samples++;
		}

		// Vertical blur
		vec4 verticalBlur = vec4(0.0);
		for (int i = -2; i <= 2; i++)
		{
			vec4 pixel = texture2D(lightMap, uv + vec2(0, i * blurRadius) / resolution);
			if (pixel.r == 0.0)
			{
				color += vec4(pixel.rgb / 2.0, pixel.a);
			}
			else
			{
				color += pixel;
			}
			samples++;
		}

		gl_FragColor = color / samples;
	}
	else
	{
		gl_FragColor = texture2D(lightMap, uv);
	}
}