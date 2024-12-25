#version 130
uniform vec2 lightPos;

void main()
{
	vec2 diff = lightPos - gl_FragCoord.xy;
	float d = length(diff);

	gl_FragColor = vec4(1.0, 0.9, 0.48, 1.1*exp(-0.01*d));
}