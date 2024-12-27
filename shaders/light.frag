#version 130

uniform vec2 lightPos;

void main()
{
	vec2 diff = lightPos - gl_FragCoord.xy;
	float d = length(diff);

	gl_FragColor = vec4(1.0, 0.59, 0.49, 1.2*exp(-0.009*d));
}