#version 450

vec2 poss[3] = vec2[]
(
	vec2(-0.5, -0.5),
	vec2(-0.5, 0.5),
	vec2(0.5, 0.0)
);

void main()
{
	gl_Position = vec4(poss[gl_VertexIndex], 0.0, 1.0);
}